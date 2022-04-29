import Game from "../Game.js";
import { assertType } from "../../Util.js";
import BinaryFile from "../../lib/BinaryFile.js";

//struct types
let ConsoleType, BootInfo;

const RAM_START = 0x80000000;

export default class RamDump {
    /** A raw RAM dump file. */
    constructor(game) {
        this.game = assertType(game, Game);
        this.app  = game.app;

        ConsoleType = this.app.types.getType('dolphin.os.ConsoleType');
        BootInfo = this.app.types.getType('dolphin.os.BootInfo');
    }

    addrToOffset(addr) {
        if(addr < RAM_START || addr >= RAM_START + this._file.size) return null;
        return addr - RAM_START;
    }

    async load(file, version='U0') {
        /** Load a save file.
         *  @param {BinaryFile} file The file to load.
         *  @param {String} version The game version this file belongs to.
         */
        this._file    = file;
        this._version = version; //game version
        const buffer  = await this._file.arrayBuffer();
        const view    = new DataView(buffer);
        this.data     = new BinaryFile(view);

        if(file.size != 24*1024*1024 //standard size
        && file.size != 48*1024*1024) { //debug size
            throw new Error("File size is incorrect");
        }

        this.bootInfo = BootInfo.fromBytes(view, 0);
    }
}
