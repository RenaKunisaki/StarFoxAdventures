import Game from "../../game/Game.js";
import { E, clearElement } from "../../lib/Element.js";
import { assertType, hex } from "../../Util.js";
import Table from "./Table.js";

export default class RamInfo {
    /** Displays info from RAM dump.
     */
    constructor(game) {
        this.game    = assertType(game, Game);
        this.app     = game.app;
        this.element = document.getElementById('tab-ramInfo');
        this.app.onRamLoaded(ram => this.refresh());
    } //constructor

    refresh() {
        const ram    = this.app.ramDump;
        const aObjs  = ram.addrToOffset(this.game.addresses.loadedObjs.address);
        const aNObjs = ram.addrToOffset(this.game.addresses.nLoadedObjs.address);

        ram.data.seek(aNObjs);
        const nObjs  = ram.data.readU32();
        console.log("nObjs", nObjs);

        const elem = E.div('ramInfo',
            E.div(null, "n objs:", nObjs.toLocaleString()));
        clearElement(this.element).append(elem);
    }
}
