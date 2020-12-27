const fieldSize = {b:1, B:1, c:1, d:8, f:4, h:2, H:2, i:4, I:4, q:8, Q:8};

export default function Struct(fields, order='>') {
    /** A binary structure definition.
     *  fields: The field definitions.
     *  order: '>' for big endian or '<' for little endian.
     */
    const _normalizedFields = {};
    let structSize = 0;
    for(let [name, field] of Object.entries(fields)) {
        let type, offs, size;
        if(field instanceof Array) {
            type = field[0];
            offs = field[1];
        }
        else {
            type = field.type;
            offs = field.offs;
        }
        if(_normalizedFields[name]) {
            throw new Error("Duplicate field name: "+String(name));
        }

        let count;
        if(typeof type == 'string') {
            count = type.match(/\d+/);
            count = (count == null) ? 1 : parseInt(count);
            type = type.match(/\d*(.+)/)[1];
            _normalizedFields[name] = {type:type, offset:offs, count:count};
            size = (type == 's') ? 1 : (count*fieldSize[type]);
        }
        else {
            count = type.count;
            size  = type.type._size;
            _normalizedFields[name] = {type:type.type, offset:offs, count:type.count};
        }
        structSize = Math.max(structSize, offs+size);
    }

    const cls = class {
        constructor(buffer, offset=0) {
            //if given a typed array, get the underlying buffer.
            if(buffer.buffer) {
                offset += buffer.byteOffset;
                buffer = buffer.buffer;
            }
            this._buffer = buffer;
            if(buffer.byteLength < structSize) {
                throw new Error("Buffer too small for type");
            }
            const view = new DataView(buffer);
            const LE = (order == '<');

            const getVal = function(field, offs) {
                offs += offset;
                try {
                    switch(field.type) {
                        case 'b': return view.getInt8(offs);
                        case 'B': return view.getUint8(offs);
                        case 'h': return view.getInt16(offs, LE);
                        case 'H': return view.getUint16(offs, LE);
                        case 'i': return view.getInt32(offs, LE);
                        case 'I': return view.getUint32(offs, LE);
                        case 'q': return view.getBigInt64(offs, LE);
                        case 'Q': return view.getBigUint64(offs, LE);
                        case 'f': return view.getFloat32(offs, LE);
                        case 'd': return view.getFloat64(offs, LE);
                        case 'c': return String.fromCharCode(view.getUint8(offs));
                        case 's': {
                            //for string, we use the field count as a maximum length.
                            let s = '';
                            while(s.length < field.count) {
                                const c = view.getUint8(offs);
                                if(c == 0) break;
                                offs++;
                                s += String.fromCharCode(c);
                            }
                            return s;
                        }
                        default: return new field.type(buffer, offs);
                    }
                }
                catch(ex) {
                    debugger;
                }
            }

            return new Proxy(this, {
                get: function(instance, key) {
                    let res = undefined;
                    const field = _normalizedFields[key];
                    if(key in instance) res = instance[key];
                    else if(field) {
                        if(field.count == 1 || field.type == 's') {
                            res = getVal(field, field.offset);
                        }
                        else {
                            let offs = field.offset;
                            res = [];
                            for(let i=0; i<field.count; i++) {
                                res.push(getVal(field, offs));
                                if(fieldSize[field.type]) offs += fieldSize[field.type];
                                else offs += field.type._size;
                            }
                        }
                        //dump value and raw data to console
                        /* if(key == 'unk1C') {
                            const b = [];
                            for(let i=0; i<4; i++) {
                                b.push(view.getUint8(field.offset+i).toString(16).padStart(2,'0'));
                            }
                            console.log("%s: %s[%d] =", key, field.type, field.count, res,
                                b.join(' '));
                        } */
                    }
                    return res;
                },
            });
        } //constructor

        _toString() {
            /** Return a string listing the values of all fields.
             *  Mainly for debugging.
             */
            const res = [];
            for(const name of Object.keys(_normalizedFields)) {
                const field =_normalizedFields[name];
                let   val   = this[name];
                const items = [];
                if(!Array.isArray(val)) val = [val];
                for(let v of val) {
                    let s=String(v);
                    switch(field.type) {
                        case 'b': case 'B': s='0x'+(v.toString(16).padStart(2,'0')); break;
                        case 'h': case 'H': s='0x'+(v.toString(16).padStart(4,'0')); break;
                        case 'i': case 'I': s='0x'+(v.toString(16).padStart(8,'0')); break;
                    }
                    items.push(s);
                }
                let t = String(field.type);
                if(typeof field.type == 'function') t = '?';
                if(field.count != 1) t += '[' + String(field.count) + ']';
                const row = [
                    t.padEnd(4),
                    field.offset.toString(16).padStart(4, '0'),
                    name,
                    items.join(', '),
                ];
                res.push(row.join('|'));
            }
            return res.join('\n');
        }
    }; //class
    cls._size = structSize;
    cls._fields = fields;

    return new Proxy(cls, {
        get: function(instance, key) {
            if(key in instance) return instance[key];
            //if(typeof key == 'number') { //lol nope
            if(!isNaN(parseInt(key))) {
                //allows eg Type[n] as a struct field
                return {type:cls, count:parseInt(key)};
            }
            console.error("Field does not exist", typeof key, key);
        },
    });
} //function Struct
