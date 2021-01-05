import {E} from '/r/js/Element.js';
import Table from '/r/js/Table.js';
import {get, hex, toHexString, hexStringToData, makeList, makeCollapsibleList, makeDescriptionAndNotes} from '/r/js/Util.js';

import Context from '../../Context.js';
import ViewControl from '../../ViewControl.js';
import LightControl from '../../LightControl.js';
import ModFile from './ModFile.js';
import MapRenderer from './MapRenderer.js';
import AssetLoader from '../AssetLoader.js';

export default class MapView {
    /** A page that attempts to render a map.
     */
    constructor(app) {
        this.app = app;
        this.element = E.div(E.canvas({id:'glCanvas'}));
    }

    getTitle() {
        const name = this.map ? this.map.internalName : this.romlistName;
        return `${name} - Map View`;
    }

    async run() {
        this.ctx = new Context('#glCanvas');
        await this.ctx.loadPrograms();

        this.romlistName = this.app.params.get('romlist');
        if(this.romlistName == null) {
            const id = this.app.params.get('id');
            this.romlistName = this.app.game.maps[id].romlistName;
        }
        this.romlist = await this.app.game.getRomList(this.romlistName);

        this.map = this.app.game.maps[this.app.params.get('id')];
        await this.map.readMapsBin();

        await this._loadBlocks();


        this.viewControl = new ViewControl(this.ctx);
        this.lightControl = new LightControl(this.ctx);
        this.element.append(
            E.div({id:'controls'},
                this.viewControl.element,
                this.lightControl.element,
                E.div({id:'glPicker'}),
            ),
        );

        this.renderer = new MapRenderer(this);
        this.ctx.setRenderer(this.renderer);

        this.viewControl.set({
            pos: {x:-2000, y:500, z:-1900},
            rot: {x:   75, y: 15, z:    0},
        });
        this.lightControl.set({
            ambColor: {r:255, g:255, b:255},
        });

        this.ctx.redraw();
        this.ctx.gx.printStats();
    }

    async _loadBlocks() {
        const trkBlk = new DataView((await get({ //XXX per-version path
            path:         `${this.app.game.version}/disc/TRKBLK.tab`,
            mimeType:     'application/octet-stream',
            responseType: 'arraybuffer',
        })).response);

        const AL = new AssetLoader(this.ctx.gx, this.app.game.version);

        const dir   = this.map.dirName;
        const mods  = {};
        this.blocks = [];
        for(let y=0; y<this.map.height; y++) {
            for(let x=0; x<this.map.width; x++) {
                let block = this.map.getBlockByPos(x, y);
                if(!block) {
                    this.blocks.push(null);
                    continue;
                }

                let modId = block.mod;
                if(modId >= 5) modId++; //dunno why the game does this
                if(!mods[modId]){
                    mods[modId] = await (new ModFile(this.app.game)).load(
                        `${dir}/mod${modId}.zlb.bin`,
                        `${dir}/mod${modId}.tab`);
                    //console.log("mod load", modId, mods[modId]);
                }
                const offs = trkBlk.getInt16(this.map.dirId * 2);
                const blk = mods[modId].blocks[block.sub+offs];
                await blk.downloadTextures(AL);
                //console.log("block %d.%d + %d", block.mod, block.sub, offs, blk);
                this.blocks.push(blk);
            }
        }
    }
}
