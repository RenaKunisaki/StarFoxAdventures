import Struct from '../../lib/Struct.js';
import IsoFile from './isofile.js';
import DOL from './dol.js';
import { Appldr } from './appldr.js';
import { Bi2Bin } from './bi2.js';
import { BootBin } from './bootbin.js';
import { FST } from './fst.js';
import { hex } from '../../Util.js';
import App from '../../app/App.js';

//boot.bin, bi2.bin, appldr, fst, main.dol, files

export const DVD_MAGIC = 0xC2339F3D;
export const BOOT_BIN_SIZE = 0x440;

export class ISO {
    constructor() {
        this.bootBin      = null;
        this.bi2bin       = null;
        this.appldr       = null;
        this.fstbin       = null;
        this.debugMonitor = null;
        this.mainDol      = null;
        this.files        = [];
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

        console.log(`Read boot.bin from 0x${hex(offset)}`);
        this.bootBin = new BootBin(buffer, offset);
        console.log("boot.bin", this.bootBin);
        if(this.bootBin.magic != DVD_MAGIC) {
            throw new Error(`Invalid DVD_MAGIC ${hex(this.bootBin.magic)}, expected ${hex(DVD_MAGIC)}`);
        }
        offset += BootBin._size;

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
        this.bi2bin = new Bi2Bin(buffer, offset);
        offset += Bi2Bin._size;

        console.log(`Read appldr   from 0x${hex(offset)}`);
        this.appldr = new Appldr(buffer, offset);
        offset += Appldr._size;

        console.log(`Read fst.bin  from 0x${hex(this.bootBin.fstOffs)}`);
        this.fstbin = new FST().read(buffer, this.bootBin.fstOffs);

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
