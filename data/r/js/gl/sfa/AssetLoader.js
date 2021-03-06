import {get} from '/r/js/Util.js';
import ModelLoader from './model/ModelLoader.js';
import TextureLoader from './model/TextureLoader.js';

export default class AssetLoader {
    /** Locates assets in SFA disc and loads them.
     */
    constructor(gx, version) {
        this.gx        = gx;
        this.version   = version;
        this.assetList = null;
        this.cache     = {};
    }

    async _getAssetList() {
        if(!this.assetList) {
            const xhr = await get({
                path: `/${this.version}/assets.xml`,
                mimeType: 'text/xml; charset=utf-8',
            });
            this.assetList = xhr.responseXML;
        }
        return this.assetList;
    }

    async loadModel(id) {
        /** Attempt to load the model with the specified ID.
         *  ID: Model ID.
         *  Returns the model, or null if it's not found.
         */
        const assetList = await this._getAssetList();
        const hid = (id & 0x7FFF).toString(16).padStart(4, '0').toUpperCase();

        //find any map that has this model.
        const map = assetList.evaluate(
            `//assets/models/model[@idx="0x${hid}"]/map`, assetList,
            null, XPathResult.ANY_UNORDERED_NODE_TYPE, null).singleNodeValue;
        if(!map) {
            console.error("No map found for model 0x%s", hid);
            return null;
        }

        //download it.
        console.log("Download model 0x%s from map:", hid, map.getAttribute('dir'));
        const TL = new ModelLoader(this.gx);
        const dir = map.getAttribute('dir');
        const path = `${this.version}/disc/${dir}/MODELS.bin`;
        const entry = parseInt(map.getAttribute('entry')) & 0x00FFFFFF;
        return await TL.loadFromModelsBin(path, entry)
    }

    async loadTexture(id) {
        /** Attempt to load the texture with the specified ID.
         *  ID: Texture ID. IDs in range 0x0000-0x7FFF specify TEX1,
         *    and range 0x8000-0xFFFF specify TEX0, as they do in the game code.
         *  Returns the texture, or null if it's not found.
         */
        const key = `tex${id}`;
        if(this.cache[key]) return this.cache[key];
        const assetList = await this._getAssetList();
        const isTex0 = ((id & 0x8000) == 0) ? '1' : '0';
        const hid = (id & 0x7FFF).toString(16).padStart(4, '0').toUpperCase();

        //find any map that has this texture.
        const map = assetList.evaluate(
            `//assets/tex${isTex0}/texture[@idx="0x${hid}"]/map`, assetList,
            null, XPathResult.ANY_UNORDERED_NODE_TYPE, null).singleNodeValue;
        if(!map) {
            console.error("No map found for texture 0x%s", hid);
            return null;
        }

        //download it.
        console.log("Download texture 0x%s from map:", hid, map.getAttribute('dir'));
        const TL = new TextureLoader(this.gx);
        const r = await TL.loadFromMap(map.getAttribute('dir'), id)
        this.cache[key] = r;
        return r;
    }
}
