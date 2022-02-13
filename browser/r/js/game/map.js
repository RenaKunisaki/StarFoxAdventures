import { int } from "../Util.js";

class Block {
    /** One block of a map.
     */
    constructor(app, map, elem) {
        this.app  = app;
        this.map  = map;
        this.mod  = null;
        this.sub  = null;
        this.x    = null;
        this.z    = null;
        this.unk1 = null;
        this.unk2 = null;

        if(elem) {
            this.mod  = int(elem.getAttribute('mod'));
            this.sub  = int(elem.getAttribute('sub'));
            this.x    = int(elem.getAttribute('x'));
            this.z    = int(elem.getAttribute('y')); //XXX
            this.unk1 = int(elem.getAttribute('unk1'));
            this.unk2 = int(elem.getAttribute('unk2'));
        }
    }
}

export default class Map {
    /** A map in the game.
     */
    constructor(app, elem) {
        this.app         = app;
        this.id          = null; //the map ID
        this.dirId       = null; //index into list of map dirs
        this.dirName     = null; //name of dir
        this.romListName = null; //name of romlist file
        this.name        = null; //internal name from MAPINFO.bin
        this.type        = null; //map type from MAPINFO.bin
        this.parentId    = null; //parent map ID
        this.playerObj   = null; //player object type (not used in this version)
        this.worldX      = null; //coords on global map grid
        this.worldZ      = null;
        this.layer       = null; //which global map grid
        this.sizeX       = null; //in blocks
        this.sizeZ       = null;
        this.originX     = null; //which block is the origin
        this.originZ     = null;
        this.links       = [null, null]; //linked map IDs
        this.blocks      = [];
        this.description = '';
        this.used        = null;

        if(elem) {
            this.id = int(elem.getAttribute('id'));
            this.dirId = int(elem.getAttribute('dirId'));
            this.type = int(elem.getAttribute('type'));
            //XXX
            this.playerObj = (int(elem.getAttribute('param3')) << 8) |
                int(elem.getAttribute('param4'));
            this.romListName = elem.getAttribute('romlist');
            this.dirName = elem.getAttribute('dir');
            this.name = elem.getAttribute('internalName');
            this.parentId = int(elem.getAttribute('parent'));
            this.worldX   = int(elem.getAttribute('worldX'));
            this.worldZ   = int(elem.getAttribute('worldY')); //XXX
            this.layer    = int(elem.getAttribute('layer'));
            this.links[0] = int(elem.getAttribute('link0'));
            this.links[1] = int(elem.getAttribute('link1'));
            this.sizeX    = int(elem.getAttribute('w'));
            this.sizeZ    = int(elem.getAttribute('h'));
            this.originX  = int(elem.getAttribute('originX'));
            this.originZ  = int(elem.getAttribute('originY')); //XXX
            this.used     = !int(elem.getAttribute('unused'));

            let eDesc = elem.getElementsByTagName('description');
            if(eDesc && eDesc[0]) this.description = eDesc[0].textContent;

            for(let block of elem.getElementsByTagName('block')) {
                this.blocks.push(new Block(this.app, this, elem));
            }
        }
    }
}
