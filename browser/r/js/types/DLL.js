import { int } from "../Util.js";

export default class DLL {
    /** A DLL in the game code.
     */
    constructor(app, eDll) {
        /** Construct a DLL from a 'dll' element from dlls.xml.
         */
        this.app       = app;
        this.id        = int(eDll.getAttribute('id'));
        this.address   = int(eDll.getAttribute('address'));
        this.dolOffs   = int(eDll.getAttribute('dolOffs'));
        this.name      = eDll.getAttribute('name');
        this.interface = eDll.getAttribute('interface');
        this.unk08     = int(eDll.getAttribute('unk08'));
        this.unk0E     = int(eDll.getAttribute('unk0E'));

        this.description = '';
        let desc = eDll.getElementsByTagName('description');
        if(desc && desc[0]) this.description = desc.textContent;

        this.notes = [];
        for(let note in eDll.getElementsByTagName('note')) {
            this.notes.push(note.textContent);
        }

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

        this.isValid = this.address >= 0x80000000 && this.address < 0x81800000;
        for(let i=0; i<2; i++) { //check constructor and destructor
            let func = this.functions[i];
            if(!func) this.isValid = false;
            else if(func.address > 0 && func.address < 0x80000000) this.isValid = false;
            else if(func.address >= 0x81800000) this.isValid = false;
        }
    }
}
