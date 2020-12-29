import {get, int, hex} from './Util.js';
import Struct from './Struct.js';
import SfaBitStream from '/r/js/gl/sfa/SfaBitStream.js';

const MapBlockHeader = Struct({
    'fileSize':      ['i',  0x08],
    'ptr_0x4c':      ['i',  0x4C], //optional; relates to hit detection
    'ptr_0x50':      ['i',  0x50], //optional; relates to hit detection
    'textures':      ['i',  0x54], //-> texture IDs
    'vtxs':          ['i',  0x58], //-> vertex positions
    'colors':        ['i',  0x5C], //-> vertex colors
    'texCoords':     ['i',  0x60], //-> vertex texture coords
    'shaders':       ['i',  0x64], //-> shader definitions
    'dlists':        ['i',  0x68], //-> DisplayListPtr
    'renderInstrs':  ['3i', 0x78], //-> render instrs for ?, ?, water
    'nRenderInstrs': ['3H', 0x84], //-> count of each
    'nVtxs':         ['H',  0x90],
    //XXX missing fields?
    'nColors':       ['H',  0x94],
    'nTexCoords':    ['H',  0x96],
    'nHits':         ['H',  0x9C], //hits aren't in this file
    'nTextures':     ['B',  0xA0],
    'nDlists':       ['B',  0xA1],
    'nShaders':      ['B',  0xA2],
    'name':          ['11s',0xA4], //size is guessed
});

export default class MapBlock {
    /** One block of a map, from the mod file.
     */
    constructor(modFile, data) {
        this.mod = modFile;
        const binView = new DataView(data);
        this.header = new MapBlockHeader(binView);
        //console.log("MapBlock header:", this.header._toString());
        this.textures     = [];
        this.vtxs         = [];
        this.colors       = [];
        this.texCoords    = [];
        this.shaders      = [];
        this.dlists       = [];
        this.renderInstrs = [];

        for(let i=0; i<this.header.nTextures; i++) {
            this.textures.push(binView.getInt32(this.header.textures + (i*4)));
        }
        for(let i=0; i<this.header.nVtxs; i++) {
            this.vtxs.push([
                binView.getInt16(this.header.vtxs + (i*6)),
                binView.getInt16(this.header.vtxs + (i*6) + 2),
                binView.getInt16(this.header.vtxs + (i*6) + 4),
            ]);
        }
        for(let i=0; i<this.header.nColors; i++) {
            //XXX confirm format
            this.colors.push(binView.getInt32(this.header.colors + (i*4)));
        }
        for(let i=0; i<this.header.nTexCoords; i++) {
            this.texCoords.push([
                binView.getInt16(this.header.texCoords + (i*4)),
                binView.getInt16(this.header.texCoords + (i*4) + 2),
            ]);
        }
        //XXX shaders
        for(let i=0; i<3; i++) {
            const offs = this.header.renderInstrs[i];
            const size = this.header.nRenderInstrs[i];
            this.renderInstrs.push(new SfaBitStream(data.slice(offs, offs+size)));
        }
    }
}
