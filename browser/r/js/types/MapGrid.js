import Struct from '../lib/Struct.js';
import Map from '../game/map.js';
const MapGridItem = Struct(
    ['h',  'x'],     //global X coord
    ['h',  'z'],     //global Z coord
    ['h',  'layer'], //layer (-2 to 2)
    ['h',  'mapId'], //map ID (if <0, marks end of list)
    ['2h', 'link'],  //linked map IDs
);

export default function parseMapGrid(app) {
    const layers    = {};
    const globalMap = app.game.iso.getFile('/globalma.bin').getData();

    for(let i=0; ; i++) {
        let entry = new MapGridItem(globalMap, i * MapGridItem._size);
        if(entry.mapId < 0) break;
        let map = app.game.maps[entry.mapId];
        if(!map) {
            console.log("No map info for ID", entry.mapId);
            //this map was missing from XML, probably because
            //we don't have XML for this version
            map         = new Map(app);
            map.id      = entry.mapId;
            map.layer   = entry.layer;
            map.x       = entry.x;
            map.z       = entry.z;
            map.sizeX   = 1;
            map.sizeZ   = 1;
            map.originX = 0;
            map.originZ = 0;
            map.blocks  = [null];
            app.game.maps[entry.mapId] = map;
        }

        if(layers[entry.layer] == undefined) layers[entry.layer] = [];
        let layer = layers[entry.layer];
        if(layer[entry.x] == undefined) layer[entry.x] = [];
        map.links = entry.link;
        map.minX =  999999; map.minZ =  999999;
        map.maxX = -999999; map.maxZ = -999999;

        //fill in the cells
        for(let x=0; x<map.sizeX; x++) {
            for(let z=0; z<map.sizeZ; z++) {
                let cx = (x + entry.x) - map.originX;
                let cz = (z + entry.z) - map.originZ;
                let iBlock = (z * map.sizeX) + x;
                if(layer[cx] == undefined) layer[cx] = [];

                let relX = x - map.originX;
                let relZ = z - map.originZ;

                //maps can overlap
                if(map.blocks[iBlock] != null || !(layer[cx][cz])) {
                    layer[cx][cz] = {
                        worldX: cx,
                        worldZ: cz,
                        layer:  entry.layer,
                        mapId:  entry.mapId,
                        link:   entry.link,
                        map:    map,
                        relX:   x - map.originX,
                        relZ:   z - map.originZ,
                        block:  map.blocks[iBlock],
                        isOrigin: x == map.originX && z == map.originZ,
                    };
                    if(relX > map.maxX) map.maxX = relX;
                    if(relZ > map.maxZ) map.maxZ = relZ;
                    if(relX < map.minX) map.minX = relX;
                    if(relZ < map.minZ) map.minZ = relZ;
                }
            }
        }
    }

    return layers;
}
