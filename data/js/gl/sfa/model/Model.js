import {get, validNumber, hexdump} from '/js/Util.js';
import SfaBitStream from '/js/gl/sfa/SfaBitStream.js';
import Texture from '/js/gl/Texture.js';
//import TextureLoader from './TextureLoader.js';
import AssetLoader from '../AssetLoader.js';
import {ModelHeader, Bone, DisplayListPtr, Shader, ShaderLayer, ModelVtxGroup} from './structs.js';

export default class Model {
    /** A character/object model file from Star Fox Adventures.
     *  This is only one of several model file formats in the game. Others
     *  include the map geometry models, and some older formats in leftover
     *  unused models stashed away in the disc.
     */
    constructor(gx) {
        this.gx           = gx;
        this.rawData      = null;
        this.header       = null;
        this.bones        = null;
        this.vtxGroups    = null;
        this.vtxs         = null;
        this.normals      = null;
        this.texCoords    = null;
        this.hitBoxes     = null;
        this.textures     = [];
        this.shaders      = null;
        this.renderInstrs = null;
        this.dlistPtrs    = null;
        this.dlists       = null;
        this.xlates       = [];
        //xlates are mtxs in the game code, combining a camera matrix
        //and a translation. since we don't need the camera matrix,
        //we'll store only the translation vector.
    }

    async load(path) {
        /** Download model file.
         *  XXX this isn't used. See ModelLoader.
         */
        const data = (await get({
            path:         path,
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response;
        return this._parseData(data);
    }

    _parseData(data) {
        /** Parse decompressed model file.
         */
        this.rawData = data;
        this.header  = new ModelHeader(this.rawData);
        console.log("Model header:", this.header._toString());

        this.bones = this._readBones();
        for(let bone of this.bones) {
            const [head, tail] = this.calcBonePos(bone, false);
            this.xlates.push(tail);
        }
        this.vtxGroups    = this._readVtxGroups();
        this.rawVtxs      = this._readVtxs();
        this.rawNormals   = this._readNormals();
        this.rawTexCoords = this._readTexCoords();
        this.dlistPtrs    = this._readDlistPtrs();
        this.dlists       = this._readDlists();
        this.renderOps    = this._readRenderOps();
        this.textureIds   = this._readTextureIds();
        this.shaders      = this._readShaders();

        //DEBUG: dump some info
        /* for(let i=0; i<this.shaders.length; i++) {
            const shader = this.shaders[i];
            //const offs = this.header.shaders + (i * Shader._size);
            //const data = this.rawData.slice(offs, offs+Shader._size);
            console.log("Shader", i, shader._toString());
        }
        for(let i=0; i<this.dlistPtrs.length; i++) {
            console.log("DlistPtr", i, this.dlistPtrs[i]._toString());
        } */

        return this;
    }

    async _downloadTextures() {
        //download textures
        const AL = new AssetLoader(this.gx);
        console.log("Texture IDs:", this.textureIds);
        for(const id of this.textureIds) {
            this.textures.push(await AL.loadTexture(id));
        }
    }

    calcBonePos(bone, relative, _depth=0) {
        /** Compute bone head/tail position relative to ancestors.
         *  bone: Which bone to compute.
         *  relative: Whether to compute final world position (by following
         *    other bones up to root) or local position (only using this bone's
         *    own transforms).
         *  Returns [head, tail], each of which are [x,y,z] coord.
         */
        console.assert(_depth < 10);
        validNumber(bone.head[0], bone.head[1], bone.head[2]);
        validNumber(bone.tail[0], bone.tail[1], bone.tail[2]);
        let head = [bone.head[0], bone.head[1], bone.head[2]];
        let tail = [bone.tail[0], bone.tail[1], bone.tail[2]];
        if(bone.parent != 0xFF) {
            if(relative) { //this makes the bones display correctly but the polygons not...
                tail[0] -= head[0]; tail[1] -= head[1]; tail[2] -= head[2];
            }
            let parent = this.bones[bone.parent];
            if(parent == undefined) {
                console.error("Bone has invalid parent", bone);
                throw new Error("Bone has invalid parent");
            }
            let [pHead, pTail] = this.calcBonePos(parent, relative, _depth+1);
            head[0] += pHead[0]; head[1] += pHead[1]; head[2] += pHead[2];
        }
        return [head, tail];
    }

    _readBones() {
        /** Read bone data from file.
         */
        const res  = [];
        let   offs = this.header.bones;
        for(let i=0; i<this.header.nBones; i++) {
            res.push(new Bone(this.rawData, offs));
            offs += Bone._size;
        }
        return res;
    }

    _readVtxGroups() {
        /** Read vertex group data from file.
         */
        const res  = [];
        let   offs = this.header.vtxGroups;
        for(let i=0; i<this.header.nVtxGroups; i++) {
            const group = new ModelVtxGroup(this.rawData, offs);
            res.push(group);
            offs += ModelVtxGroup._size;
            const bone0 = this.bones[group.bone0];
            const bone1 = this.bones[group.bone1];
            //the weight isn't scaled by 255 like you might expect.
            const wgt0  = validNumber(group.weight) / 4;
            const wgt1  = 1.0 - wgt0;
            const [head0, tail0] = this.calcBonePos(bone0, true);
            const [head1, tail1] = this.calcBonePos(bone1, true);
            //we do have to scale these because we scale the vtxs
            //but we don't scale the bone transforms because they're
            //getting the vertex groups into place
            const x = (((tail0[0] * wgt0) + (tail1[0] * wgt1))) / 256.0;
            const y = (((tail0[1] * wgt0) + (tail1[1] * wgt1))) / 256.0;
            const z = (((tail0[2] * wgt0) + (tail1[2] * wgt1))) / 256.0;
            this.xlates.push([validNumber(x), validNumber(y), validNumber(z)]);
        }
        return res;
    }

    _readVtxs() {
        /** Read raw vertex data from file.
         */
        //don't bother interpreting it here. we can just chuck the raw data
        //at GX and let it figure it out.
        return this.rawData.slice(this.header.vtxs,
            this.header.vtxs + (this.header.nVtxs * 6));
    }

    _readNormals() {
        /** Read raw normal data from file.
         */
        return this.rawData.slice(this.header.normals,
            this.header.normals + (this.header.nNormals * 6));
    }

    _readTexCoords() {
        /** Read raw texture coordinate data from file.
         */
        return this.rawData.slice(this.header.texCoords,
            this.header.texCoords + (this.header.nTexCoords * 4));
    }

    _readDlistPtrs() {
        /** Read display list pointers from file.
         */
        const res  = [];
        let   offs = this.header.dlists;
        for(let i=0; i<this.header.nDlists; i++) {
            res.push(new DisplayListPtr(this.rawData, offs));
            offs += 0x1C;
        }
        return res;
    }

    _readDlists() {
        /** Read display lists from file.
         */
        const res = [];
        for(let i=0; i<this.dlistPtrs.length; i++) {
            const offs = this.dlistPtrs[i].offset;
            const size = this.dlistPtrs[i].size;
            res.push(this.rawData.slice(offs, offs+size));
        }
        return res;
    }

    _readRenderOps() {
        /** Read render instructions from file.
         */
        const offs = this.header.renderInstrs;
        const size = this.header.nRenderInstrs;
        return new SfaBitStream(this.rawData.slice(offs, offs+size));
    }

    _readTextureIds() {
        /** Read texture list from file.
         */
        const offs = this.header.textures;
        const res  = [];
        const data = new DataView(this.rawData);
        for(let i=0; i<this.header.nTextures; i++) {
            res.push(data.getInt32(offs+(i*4)));
        }
        return res;
    }

    _readShaders() {
        /** Read shader definitions from file.
         */
        const res  = [];
        for(let i=0; i<this.header.nShaders; i++) {
            const offs = this.header.shaders + (i * Shader._size);
            res.push(new Shader(this.rawData.slice(offs, offs+Shader._size)));
        }
        return res;
    }
}
