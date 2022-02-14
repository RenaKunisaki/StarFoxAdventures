import Struct from '../lib/Struct.js';
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

        if(layers[entry.layer] == undefined) layers[entry.layer] = [];
        let layer = layers[entry.layer];
        if(layer[entry.x] == undefined) layer[entry.x] = [];
        map.links = entry.link;

        //fill in the cells
        for(let x=0; x<map.sizeX; x++) {
            for(let z=0; z<map.sizeZ; z++) {
                let cx = (x + entry.x) - map.originX;
                let cz = (z + entry.z) - map.originZ;
                let iBlock = (z * map.sizeX) + x;
                if(layer[cx] == undefined) layer[cx] = [];
                //maps can overlap
                if(map.blocks[iBlock] != null || !(layer[cx][cz])) {
                    layer[cx][cz] = {
                        worldX: cx,
                        worldZ: cz,
                        layer:  entry.layer,
                        mapId:  entry.mapId,
                        link:   entry.link,
                        map:    map,
                        relX:   x,
                        relZ:   z,
                        block:  map.blocks[iBlock],
                        isOrigin: x == map.originX && z == map.originZ,
                    };
                }
            }
        }
    }

    return layers;
}