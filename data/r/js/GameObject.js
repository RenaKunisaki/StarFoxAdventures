import {int, getDescriptionAndNotes} from './Util.js';
import {ObjectParam} from './Dll.js';

export default class GameObject {
    /** An object/actor in the game.
     */
    constructor(game, eObj) {
        /** Construct a GameObject from a 'object' element from objects.xml.
         */
        this.game        = game;
        this.id          = int(eObj.getAttribute('id'));
        this.name        = eObj.getAttribute('name');
        this.defNo       = int(eObj.getAttribute('def'));
        this.dllId       = int(eObj.getAttribute('dll'));
        this.nPlayerObjs = int(eObj.getAttribute('nPlayerObjs'));
        this.category    = int(eObj.getAttribute('cat'));
        this.type        = eObj.getAttribute('type');
        this.maps        = {};
        this.models      = [];
        this.modelIds    = [];
        this.params      = [];
        this.spawns      = [];
        this.dll         = game.dlls[this.dllId];
        this.sequences   = [];
        this.paramLength = null;

        for(let eMap of eObj.getElementsByTagName('map')) {
            this.maps[int(eMap.getAttribute('id'))] = int(eMap.getAttribute('count'));
        }
        for(let eModel of eObj.getElementsByTagName('model')) {
            let id = int(eModel.getAttribute('id'));
            this.modelIds.push(id);
            this.models.push(game.models[id]);
        }

        //Ideally, object params are specified under the object's DLL.
        //But some entries in objects.xml have them as well.
        //This is mainly because I changed that after making the files,
        //but also, an object could theoretically have params but no DLL.
        let eParams = eObj.getElementsByTagName('params')[0];
        if(eParams) {
            this.paramLength = int(eParams.getAttribute('length'));
            for(let eParam of eParams.getElementsByTagName('param')) {
                this.params.push(new ObjectParam(eParam));
            }
        }
        if(this.dll) {
            for(let param of this.dll.objParams) {
                this.params.push(param);
            }
        }

        this.exampleParams = null;
        let example = eObj.getElementsByTagName('exampleParams');
        if(example && example[0]) this.exampleParams = example[0].textContent;

        for(let eSpawn of eObj.getElementsByTagName('spawn')) {
            this.spawns.push(eSpawn.textContent);
        }

        for(let eSeq of eObj.getElementsByTagName('seq')) {
            this.sequences.push(int(eSeq.getAttribute('id')));
        }

        getDescriptionAndNotes(this, eObj);
    }
}
