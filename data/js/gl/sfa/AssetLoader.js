import {get} from '/js/Util.js';
import TextureLoader from './model/TextureLoader.js';

export default class AssetLoader {
    /** Locates assets in SFA disc and loads them.
     */
    constructor(gx) {
        this.gx        = gx;
        this.assetList = null;
    }

    async _getAssetList() {
        if(!this.assetList) {
            const xhr = await get({
                path: 'assets.xml',
                mimeType: 'text/xml; charset=utf-8',
            });
            this.assetList = xhr.responseXML;
        }
        return this.assetList;
    }

    async loadTexture(id) {
        /** Attempt to load the texture with the specified ID.
         *  ID: Texture ID. IDs in range 0x0000-0x7FFF specify TEX1,
         *    and range 0x8000-0xFFFF specify TEX0, as they do in the game code.
         *  Returns the texture, or null if it's not found.
         */
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
        return await TL.loadFromMap(map.getAttribute('dir'), id)
    }
}
