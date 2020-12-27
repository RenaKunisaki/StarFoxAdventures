import {getDescriptionAndNotes} from './Util.js';

export class ObjectParam {
    /** A parameter for a GameObject.
     *  This class is exported because GameObject needs to be able to construct
     *  it too, since objects could have their own parameter definitions.
     */
    constructor(eParam) {
        this.name   = eParam.getAttribute('name');
        this.offset = parseInt(eParam.getAttribute('offset'));
        this.type   = eParam.getAttribute('type');
        getDescriptionAndNotes(this, eParam);
    }
}

class Param {
    /** A function parameter.
     */
    constructor(eParam) {
        /** Construct a Param from a 'param' element from dlls.xml.
         */
        this.reg       = eParam.getAttribute('reg');
        this.stackOffs = eParam.getAttribute('stackOffs');
        this.type      = eParam.getAttribute('type');
        this.name      = eParam.getAttribute('name');
        this.allowNull = eParam.getAttribute('allowNull') == '1';
        getDescriptionAndNotes(this, eParam);
    }
}

class Function {
    /** A function in a DLL.
     */
    constructor(eFunc) {
        /** Construct a Function from a 'function' element from dlls.xml.
         */
        this.address = parseInt(eFunc.getAttribute('address'));
        this.idx     = parseInt(eFunc.getAttribute('idx'));
        this.name    = eFunc.getAttribute('name');
        this.isValid = eFunc.getAttribute('valid') != '0';
        this.isStub  = eFunc.getAttribute('stub')  == '1';
        if(this.isStub) {
            this.stubReturn = eFunc.getAttribute('return');
        }

        this.params = [];
        let eParams = eFunc.getElementsByTagName('params');
        if(eParams && eParams[0]) {
            this.knownParams = true;
            for(let eParam of eParams[0].getElementsByTagName('param')) {
                this.params.push(new Param(eParam));
            }
        }
        else this.knownParams = false;

        getDescriptionAndNotes(this, eFunc);
    }
}

export class Dll {
    /** A DLL in the game code.
     */

    constructor(game, eDll) {
        /** Construct a Dll from a 'dll' element from dlls.xml.
         */
        this.game      = game;
        this.id        = parseInt(eDll.getAttribute('id'));
        this.address   = parseInt(eDll.getAttribute('address'));
        this.dolOffs   = parseInt(eDll.getAttribute('dolOffs'));
        this.interface = eDll.getAttribute('interface');
        this.name      = eDll.getAttribute('name');
        this.unk08     = parseInt(eDll.getAttribute('unk08'));
        this.unk0E     = parseInt(eDll.getAttribute('unk0E'));
        this.prev      = parseInt(eDll.getAttribute('prev'));
        this.next      = parseInt(eDll.getAttribute('next'));

        //const link = E.a(null, {href:`dllinfo.html?ver=${ver}&id=${id}`}, dispId);
        let eFuncs = eDll.getElementsByTagName('functions')[0];
        eFuncs = eFuncs ? eFuncs.getElementsByTagName('function') : [];

        //make list of functions and non-stub functions
        this.functions = [];
        this.nNonStubs = 0;
        for(let eFunc of eFuncs) {
            let func = new Function(eFunc);
            this.functions.push(func);
            if(func.isValid && !func.isStub) this.nNonStubs++;
        }

        getDescriptionAndNotes(this, eDll);

        this.objParams = [];
        let eParams = eDll.getElementsByTagName('objparams')[0];
        if(eParams) {
            this.objParamLength = parseInt(eParams.getAttribute('length'));
            for(let eParam of eParams.getElementsByTagName('param')) {
                this.objParams.push(new ObjectParam(eParam));
            }
        }
    }
}
