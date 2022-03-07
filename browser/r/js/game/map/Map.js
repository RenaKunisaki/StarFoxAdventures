export default class Map {
    /** A map in the game.
     *  @note Map IDs can be confusing because the game refers to them by
     *   different IDs in different contexts:
     *   - id: index into MAPINFO.bin
     *   - dirId: index into directory name list in DOL
     *   Also, there are some files left on the disc from deleted maps;
     *   we assign these negative IDs if they don't have an ID defined.
     *   Also, not all maps have all information; for example, there are
     *   several entries in MAPINFO.bin that have a corresponding romlist
     *   file but no asset directory.
     */
    constructor(app, params={}) {
        this.app         = app;
        this.id          = params.id;          //the map ID
        this.dirId       = params.dirId;       //index into list of map dirs
        this.dirName     = params.dirName;     //name of asset dir
        this.romListName = params.romListName; //name of romlist file
        this.romListSize = params.romListSize; //size to allocate for romlist
        this.romList     = params.romList;     //the RomList instance
        this.name        = params.name;        //internal name from MAPINFO.bin
        this.type        = params.type;        //map type from MAPINFO.bin
        this.parentId    = params.parentId;    //parent map ID
        this.links       = params.links;       //linked map IDs
        this.objType     = params.objType;     //player object type (not used in final)
        this.worldX      = params.worldX;      //coords on global map grid
        this.worldZ      = params.worldZ;
        this.layer       = params.layer;       //which global map grid
        this.sizeX       = params.sizeX;       //in blocks
        this.sizeZ       = params.sizeZ;
        this.originX     = params.originX;     //which block is the origin
        this.originZ     = params.originZ;
        this.blocks      = params.blocks;      //list of blocks
        this.unk08       = params.unk08;
        this.unk0C       = params.unk0C;
        this.unk1D       = params.unk1D;
        this.unk1E       = params.unk1E;
        //following are added manually, not present in the game data
        this.description = params.description; //description
        this.isUsed      = params.isUsed;      //is used in game?
    }

    getBlock(x, z) {
        if(this.blocks == undefined) return null;
        let idx = (z * this.sizeX) + x;
        return this.blocks[idx];
    }

    addBlock(block) {
        if(this.blocks == undefined) this.blocks = [];
        this.blocks.push(block);
    }
}
