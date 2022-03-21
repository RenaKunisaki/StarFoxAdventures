import DOL from './dol.js';
import { FST } from './fst.js';
import { hex } from '../../Util.js';

//boot.bin, bi2.bin, appldr, fst, main.dol, files

export const DVD_MAGIC = 0xC2339F3D;
export const BOOT_BIN_SIZE = 0x440;

//struct types
let Appldr, Bi2Bin, BootBin;

export class ISO {
    constructor(app) {
        this.app          = app;
        this.bootBin      = null;
        this.bi2bin       = null;
        this.appldr       = null;
        this.fstbin       = null;
        this.debugMonitor = null;
        this.mainDol      = null;
        this.files        = [];

        Appldr  = this.app.types.getType('iso.Appldr');
        Bi2Bin  = this.app.types.getType('iso.Bi2Bin');
        BootBin = this.app.types.getType('iso.BootBin');
    }

    readBuffer(buffer, offset=0) {
        /** Read entire ISO file from buffer.
         */
        if(buffer.buffer) {
            //if given a typed array, get the underlying buffer.
            offset += buffer.byteOffset;
            buffer  = buffer.buffer;
        }
        console.log("Buffer is", buffer);
        const view = new DataView(buffer);

        console.log(`Read boot.bin from 0x${hex(offset)}`);
        this.bootBin = BootBin.fromBytes(view, offset);
        console.log("boot.bin", this.bootBin);
        if(this.bootBin.magic != DVD_MAGIC) {
            console.error(`Invalid DVD_MAGIC ${hex(this.bootBin.magic)}, expected ${hex(DVD_MAGIC)}`);
            throw new Error("Not a GameCube ISO file");
        }
        offset += BootBin.size;

        console.log("Game code:", this.bootBin.gameCode);
        console.log("Company code:", this.bootBin.company);
        console.log("Disc #:", this.bootBin.discNo);
        console.log("Version:", this.bootBin.version);
        console.log("Streaming:", this.bootBin.audioStreaming,
            this.bootBin.streamBufSize);
        console.log("Title:", this.bootBin.gameName);
        console.log("Debug Monitor:", hex(this.bootBin.debugMonitorOffs),
            hex(this.bootBin.debugMonitorAddr));
        console.log("main.dol:", hex(this.bootBin.mainDolOffs));
        console.log("FST: offset", hex(this.bootBin.fstOffs),
            "size", hex(this.bootBin.fstSize),
            "max", hex(this.bootBin.maxFstSize),
            "addr", hex(this.bootBin.fstAddr));
        console.log("Files:", hex(this.bootBin.fileOffset),
            hex(this.bootBin.fileLength));

        console.log(`Read bi2.bin  from 0x${hex(offset)}`);
        this.bi2bin = Bi2Bin.fromBytes(view, offset);
        offset += Bi2Bin.size;

        console.log(`Read appldr   from 0x${hex(offset)}`);
        this.appldr = Appldr.fromBytes(view, offset);
        offset += Appldr.size;

        console.log(`Read fst.bin  from 0x${hex(this.bootBin.fstOffs)}`);
        this.fstbin = new FST(this.app).read(buffer, this.bootBin.fstOffs);

        //size is found by adding up the sections, so not known here.
        //not sure how it gets loaded, probably appldr just loads a fixed size.
        console.log(`Read main.dol from 0x${hex(this.bootBin.mainDolOffs)}`);
        this.mainDol = new DOL("main.dol", this.bootBin.mainDolOffs, 0,
            buffer, true);

        //XXX debug monitor?

        console.log(`Read files    from 0x${hex(this.bootBin.mainDolOffs+this.mainDol.size)}`);
        this.files = this.fstbin.files;

        return this;
    }

    getFile(path) {
        //Retrieve file from ISO.
        for(const file of this.files) {
            if(file.path == path) return file;
        }
        return null;
    }
}
