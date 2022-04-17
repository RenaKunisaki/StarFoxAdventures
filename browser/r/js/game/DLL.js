import { assertType, int, hex } from "../Util.js";
import Game from "./Game.js";

//XXX display logic doesn't belong here
const objParamTypeFmt = {
    s8:          (game,val) => `0x${hex(val, 2)}`,
    byte:        (game,val) => `0x${hex(val, 2)}`,
    u8:          (game,val) => `0x${hex(val, 2)}`,
    ubyte:       (game,val) => `0x${hex(val, 2)}`,
    //undefined:   (game,val) => `0x${hex(val, 2)}`,
    undefined1:  (game,val) => `0x${hex(val, 2)}`,
    s16:         (game,val) => `0x${hex(val, 4)}`,
    short:       (game,val) => `0x${hex(val, 4)}`,
    u16:         (game,val) => `0x${hex(val, 4)}`,
    ushort:      (game,val) => `0x${hex(val, 4)}`,
    undefined2:  (game,val) => `0x${hex(val, 4)}`,
    s32:         (game,val) => `0x${hex(val, 8)}`,
    int:         (game,val) => `0x${hex(val, 8)}`,
    u32:         (game,val) => `0x${hex(val, 8)}`,
    uint:        (game,val) => `0x${hex(val, 8)}`,
    undefined4:  (game,val) => `0x${hex(val, 8)}`,
    s64:         (game,val) => `0x${hex(val,16)}`,
    u64:         (game,val) => `0x${hex(val,16)}`,
    float:       (game,val) => val.toFixed(2),
    double:      (game,val) => val.toFixed(2),
    GameBit:     (game,val) => `0x${hex(val, 4)}`,
    GameBit16:   (game,val) => `0x${hex(val, 4)}`,
    GameBit32:   (game,val) => `0x${hex(val, 4)}`,
    ObjUniqueId: (game,val) => `0x${hex(val, 8)}`,
    ObjDefEnum:  (game,val) => {
        const obj = game.getObject(val);
        const name = obj ? obj.name : '?';
        return `0x${hex(val,4)} ${name}`;
    },
    MapDirIdx8: (game,val) => {
        if(game.mapDirs[val]) {
            return `0x${hex(val,2)} ${game.mapDirs[val].name}`;
        }
        else return `0x${hex(val,2)} ?`;
    },
};

export default class DLL {
    /** A DLL in the game code.
     */
    constructor(game, eDll) {
        /** Construct a DLL from a 'dll' element from dlls.xml.
         */
        this.game      = assertType(game, Game);
        this.app       = game.app;
        this.id        = int(eDll.getAttribute('id'));
        this.address   = int(eDll.getAttribute('address'));
        this.dolOffs   = int(eDll.getAttribute('dolOffs'));
        this.name      = eDll.getAttribute('name');
        this.interface = eDll.getAttribute('interface');

        this.description = '';
        let desc = eDll.getElementsByTagName('description');
        if(desc && desc[0]) this.description = desc[0].textContent;

        this.notes = [];
        for(let note in eDll.getElementsByTagName('note')) {
            this.notes.push(note.textContent);
        }

        this._readDol();
        this._readFuncs(eDll);
        this._readObjParamsFromXml(eDll);

        this.isValid = this.address >= 0x80000000 && this.address < 0x81800000;
        for(let i=0; i<2; i++) { //check constructor and destructor
            let func = this.functions[i];
            if(!func) this.isValid = false;
            else if(func.address > 0 && func.address < 0x80000000) this.isValid = false;
            else if(func.address >= 0x81800000) this.isValid = false;
        }
    }

    _readDol() {
        //read data from the DOL file instead of XML
        if(!this.game.iso) return;
        const dol       = this.game.iso.mainDol;
        const data      = dol.getData();
        const gDllsAddr = this.game.addresses.g_dlls.address;
        const dllOffs   = dol.addrToOffset(gDllsAddr + (this.id*4)); //file offset of &g_dlls[this.id]
        const aPtr      = data.getUint32(dllOffs); //address of this DLL
        const aOffs     = dol.addrToOffset(aPtr); //file offset of this DLL
        this.address    = aPtr;
        this.dolOffs    = aOffs;
        //this.nFuncs     = data.getUint16(aOffs+0xC); //export count
        //this value is wrong for some deleted DLLs
    }

    _readFuncs(eDll) {
        //XXX get func info from interface
        this.functions = [];
        for(let eFunc of eDll.getElementsByTagName('function')) {
            let func = {
                idx:     int(eFunc.getAttribute('idx')),
                address: int(eFunc.getAttribute('address')),
                name:    eFunc.getAttribute('name'),
                valid:   eFunc.getAttribute('valid'),
                stub:    eFunc.getAttribute('stub'),
                params:  [],
                return:  null,
            };
            if(func.valid == undefined) func.valid = true;
            else func.valid = (func.valid == 1);
            if(func.stub == undefined) func.stub = false;
            else func.stub = (func.stub == 1);

            let eReturn = eFunc.getElementsByTagName('return');
            if(eReturn && eReturn[0]) {
                func.return = eReturn[0].getAttribute('type');
            }

            for(let eParam of eFunc.getElementsByTagName('param')) {
                let param = {
                    name: eParam.getAttribute('name'),
                    reg:  eParam.getAttribute('reg'),
                    type: eParam.getAttribute('type'),
                    description: '',
                };
                let desc = eParam.getElementsByTagName('description');
                if(desc && desc[0]) param.description = desc[0].textContent;
                func.params.push(param);
            }
            this.functions.push(func);
        }
    }

    _readObjParamsFromXml(eDll) {
        this.objParams = null;
        this.objParamOffsets = null;
        const eParams = eDll.getElementsByTagName('objparams').item(0);
        if(!eParams) return;
        let dllName = eDll.getAttribute('name');
        if(!dllName) dllName = eDll.getAttribute('id');

        this.objParams = {};
        this.objParamOffsets = {};
        let maxOffs = 0;
        for(let eParam of eParams.getElementsByTagName('field')) {
            let param = {
                name:   eParam.getAttribute('name'),
                offset: int(eParam.getAttribute('offset')),
                type:   eParam.getAttribute('type'),
            };
            if(param.name == undefined) param.name = `_${hex(param.offset)}`;
            let eDesc = eParam.getElementsByTagName('description').item(0);
            if(eDesc) param.description = eDesc.textContent;
            if(this.objParamOffsets[param.offset]) {
                //we do this sometimes when the same field has different
                //meanings depending on some other param
                console.warn(`Multiple params assigned to offset 0x${hex(param.offset)} of DLL ${dllName}`);
            }
            this.objParams[param.name] = param;
            this.objParamOffsets[param.offset] = param;
            if(param.offset > maxOffs) maxOffs = param.offset;
        }

        //create a Struct
        const ns = this.app.types.getType('sfa');
        this.objParamStruct = this.app.types.parseStruct(eParams, ns);
    }

    readObjParams(data) {
        /** Read object params from data.
         *  @param {DataView} data view to read from.
         *  @returns {object} parameter name => value, or null.
         *  @note Expects data to contain only the bytes from the beginning of
         *    the 0x18-byte common ObjDef to the end of the parameters.
         */
        if(!this.objParams) return null;
        let values;
        try {
            values = this.objParamStruct.fromBytes(data);
        }
        catch(ex) {
            if(!(ex instanceof RangeError)) throw ex;
            console.error("Out of range reading params for DLL", this, data);
            return null;
        }

        //XXX this is confusing, sometimes we're using this and
        //sometimes we're just using the struct directly.
        const result = {};
        for(let field of this.objParamStruct.fields) {
            const val = values[field.name];
            let disp = val.toString();

            let type = field.type;
            if(type) {
                let found = true;
                if(type.valueToString) disp = type.valueToString(val);
                else if(type.type.valueToString) disp = type.type.valueToString(val);
                else if(objParamTypeFmt[type]) {
                    disp = objParamTypeFmt[type](this.game, val);
                }
                else found = false;
                if(found && disp != val && typeof(val) != 'object') {
                    disp = `${disp} (${val})`;
                }
            }

            result[field.name] = {
                //type:    field,
                param:   this.objParams[field.name],
                value:   val,
                display: disp,
            };
        }
        return result;
    }
}
