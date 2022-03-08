import { hex, int } from "../../Util.js";

export class CommandBase {
    /** A control code in a GameText string.
     *  In the game's files, these are represented as a particular
     *  Unicode character (in the Private Use range) followed by some
     *  number of u16 parameter values.
     *  Since the number of parameters depends on the control code, and
     *  the parameter bytes can be zeroes, it's necessary to parse them
     *  while reading the string.
     *  In a Phrase object, instances of the classes that extend this
     *  class are used to represent the control codes.
     *  In XML, a tag named for the class is used, with each parameter
     *  being an attribute.
     */
    constructor(...params) {
        //"this.params" doesn't work here but "this.constructor.params" does...
        console.assert(params.length == this.constructor.params.length);
        for(let i=0; i<params.length; i++) {
            this[this.constructor.params[i]] = int(params[i]);
        }
        this.chr = this.constructor.chr;
    }
}

export const Command = {
    //keys must be lowercase since they're used as XML tag names.
    seqnum: class extends CommandBase {
        static chr    = 0xE000;
        static params = ['id'];
        toString() { return `[SEQ ${this.id}]` } //for debug/unformatted viewing
    },
    time: class extends CommandBase {
        static chr    = 0xE018;
        static params = ['unk1', 'time', 'unk3'];
        toString() { return `[TIME ${this.unk1}, ${this.time}, ${this.unk3}]` }
    },
    hint: class extends CommandBase {
        static chr    = 0xE020;
        static params = ['id'];
        toString() { return `[HINT ${this.id}]` }
    },
    unkf8f2: class extends CommandBase {
        static chr    = 0xF8F2;
        static params = ['unk1', 'unk2'];
        toString() { return `[F8F2 ${this.unk1} ${this.unk2}]` }
    },
    unkf8f3: class extends CommandBase {
        static chr    = 0xF8F3;
        static params = ['unk1', 'unk2'];
        toString() { return `[F8F3 ${this.unk1} ${this.unk2}]` }
    },
    scale: class extends CommandBase {
        static chr    = 0xF8F4;
        static params = ['scale'];
        toString() { return `[SCALE ${Math.round(this.scale/2.56)}%]` }
    },
    unkf8f5: class extends CommandBase {
        static chr    = 0xF8F5;
        static params = ['unk1'];
        toString() { return `[F8F5 ${this.unk1}]` }
    },
    unkf8f6: class extends CommandBase {
        static chr    = 0xF8F6;
        static params = ['unk1'];
        toString() { return `[F8F6 ${this.unk1}]` }
    },
    font: class extends CommandBase {
        static chr    = 0xF8F7;
        static params = ['id'];
        toString() { return `[FONT ${this.id}]` }
    },
    justleft: class extends CommandBase {
        static chr    = 0xF8F8;
        static params = [];
        toString() { return `[JUSTLEFT]` }
    },
    justright: class extends CommandBase {
        static chr    = 0xF8F9;
        static params = [];
        toString() { return `[JUSTRIGHT]` }
    },
    justcenter: class extends CommandBase {
        static chr    = 0xF8FA;
        static params = [];
        toString() { return `[JUSTCENTER]` }
    },
    justfull: class extends CommandBase {
        static chr    = 0xF8FB;
        static params = [];
        toString() { return `[JUSTFULL]` }
    },
    unkf8fc: class extends CommandBase {
        static chr    = 0xF8FC;
        static params = [];
        toString() { return `[F8FC]` }
    },
    unkf8fd: class extends CommandBase {
        static chr    = 0xF8FD;
        static params = [];
        toString() { return `[F8FD]` }
    },
    unkf8fe: class extends CommandBase {
        static chr    = 0xF8FE;
        static params = [];
        toString() { return `[F8FE]` }
    },
    color: class extends CommandBase {
        static chr    = 0xF8FF;
        static params = ['r', 'g', 'b', 'a'];
        toString() { return `[COLOR #${hex(this.r,2)}${hex(this.g,2)}${hex(this.b,2)}${hex(this.a,2)}]` }
    },
}; //Command

export const CommandById = {};
for(let cls of Object.values(Command)) {
    CommandById[cls.chr] = cls;
}
