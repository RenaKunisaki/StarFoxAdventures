import { assertType, int, hex } from "../Util.js";
import Struct, { parseType } from "../lib/Struct.js";
import { GhidraTypes } from "../types/GhidraTypes.js";
import Game from "./Game.js";

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
        //XXX these fields shouldn't exist (were based on outdated info)
        //this.unk08     = int(eDll.getAttribute('unk08'));
        //this.unk0E     = int(eDll.getAttribute('unk0E'));

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
        for(let eParam of eParams.getElementsByTagName('param')) {
            let param = {
                name:   eParam.getAttribute('name'),
                offset: int(eParam.getAttribute('offset')),
                type:   eParam.getAttribute('type'),
            };
            if(param.name == undefined) param.name = `unk${hex(param.offset)}`;
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
        const structDef = [];
        let offs = 0x18; //end of common objdef
        while(offs <= maxOffs) {
            let prevOffs = offs;
            let param = this.objParamOffsets[offs];
            if(param) {
                console.assert(GhidraTypes[param.type]);
                const type = GhidraTypes[param.type].type;
                structDef.push([type, param.name]);
                const typeInfo = parseType(type);
                offs += typeInfo.size * typeInfo.count;
            }
            else {
                structDef.push(['b', `_${hex(offs)}`]);
                offs++;
            }
            for(let i=prevOffs+1; i<offs; i++) {
                if(this.objParamOffsets[i]) {
                    const p = this.objParamOffsets[i];
                    console.warn(`Param ${p.name} overlaps another param in DLL ${dllName}`);
                }
            }
        }
        //console.log("Creating struct", structDef);
        this.objParamStruct = Struct(...structDef);
    }

    readObjParams(data) {
        /** Read object params from data.
         *  @param {DataView} data view to read from.
         *  @returns {object} parameter name => value, or null.
         *  @note Expects data to contain only the bytes following
         *    the 0x18-byte common ObjDef.
         */
        if(!this.objParams) return null;
        const values = new this.objParamStruct(data);
        const result = {};
        for(let [name,param] of Object.entries(this.objParams)) {
            const val = values[name];
            result[name] = {
                param: param,
                value: val,
            };
            //make displayed value
            const type = GhidraTypes[param.type];
            let disp = String(result[name].value);
            if(type.disp) disp = type.disp(result[name].value);
            else switch(param.type) {
                case 'GameBit': case 'GameBit16': case 'GameBit32': {
                    disp = `0x${hex(val,4)}`; //XXX link to list
                    break;
                }
                case 'ObjDefEnum': {
                    const obj = this.game.getObject(val);
                    const name = obj ? obj.name : '?';
                    disp = `0x${hex(val,4)} ${name}`;
                    break;
                }
                case 'MapDirIdx8': {
                    if(this.game.mapDirs[val]) {
                        disp = `0x${hex(val,2)} ${this.game.mapDirs[val].name}`;
                    }
                    else disp = `0x${hex(val,2)} ?`;
                    break;
                }
            }
            result[name].display = disp;
        }
        return result;
    }
}
