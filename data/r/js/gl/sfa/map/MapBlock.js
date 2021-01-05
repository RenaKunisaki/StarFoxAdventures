import {get, int, hex, hexdump} from '/r/js/Util.js';
import Struct from '/r/js/Struct.js';
import {Shader, ShaderLayer, TextureHeader, DisplayListPtr} from '../structs.js';
import SfaBitStream from '../SfaBitStream.js';

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
    //'normals':       ['i',  0x6C], // guessed
    'renderOps':     ['3i', 0x78], //-> render instrs for main, reflective, water
    'nRenderOps':    ['3H', 0x84], //-> count of each
    'nVtxs':         ['H',  0x90],
    //'nNormals':      ['H',  0x92], //guessed
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
        this.textureIds   = [];
        this.vtxs         = [];
        this.colors       = [];
        this.texCoords    = [];
        this.shaders      = [];
        this.dlists       = [];
        this.renderOps    = [];

        for(let i=0; i<this.header.nDlists; i++) {
            const ptr = new DisplayListPtr(data, this.header.dlists + (i * 0x1C));
            this.dlists.push(data.slice(ptr.offset, ptr.offset+ptr.size));
        }
        for(let i=0; i<this.header.nTextures; i++) {
            this.textureIds.push(binView.getInt32(this.header.textures + (i*4)));
        }
        /* for(let i=0; i<this.header.nVtxs; i++) {
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
        } */
        this.vtxs = data.slice(this.header.vtxs, this.header.vtxs+this.header.nVtxs*6);
        this.colors = data.slice(this.header.colors, this.header.colors+this.header.nColors*4);
        //this.normals = data.slice(this.header.normals, this.header.normals+this.header.nNormals*6);
        this.texCoords = data.slice(this.header.texCoords, this.header.texCoords+this.header.nTexCoords*4);
        for(let i=0; i<this.header.nShaders; i++) {
            const offs = this.header.shaders + (i * Shader._size);
            this.shaders.push(new Shader(data.slice(offs, offs+Shader._size)));
        }
        for(let i=0; i<3; i++) {
            const offs = this.header.renderOps[i];
            const size = this.header.nRenderOps[i];
            this.renderOps.push(new SfaBitStream(data.slice(offs, offs+size)));
        }

        //console.log("Block dlist data:", hexdump(this.dlists[0]));
    }

    async downloadTextures(assetLoader) {
        //console.log("Texture IDs:", this.textureIds);
        for(const id of this.textureIds) {
            this.textures.push(await assetLoader.loadTexture(id));
        }
    }
}
