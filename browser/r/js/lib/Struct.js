/** JavaScript struct library
 *  2022 Rena Kunisaki
 *  Danger, dragons ahead.
 */
const fieldTypes = {
    //type: {size, func}
    //type is the character used to specify this type.
    //size is the number of bytes wide this type is.
    //func is the function used to read this type from a DataView.
    //func's params
    //  view: the view to read from.
    //  offs: byte offset to read from.
    //  LE:   true for little-endian, false for big-endian.
    b: {size:1, func:(view, offs, LE) => view.getInt8     (offs), },
    B: {size:1, func:(view, offs, LE) => view.getUint8    (offs), },
    h: {size:2, func:(view, offs, LE) => view.getInt16    (offs, LE), },
    H: {size:2, func:(view, offs, LE) => view.getUint16   (offs, LE), },
    i: {size:4, func:(view, offs, LE) => view.getInt32    (offs, LE), },
    I: {size:4, func:(view, offs, LE) => view.getUint32   (offs, LE), },
    q: {size:8, func:(view, offs, LE) => view.getBigInt64 (offs, LE), },
    Q: {size:8, func:(view, offs, LE) => view.getBigUint64(offs, LE), },
    f: {size:4, func:(view, offs, LE) => view.getFloat32  (offs, LE), },
    d: {size:8, func:(view, offs, LE) => view.getFloat64  (offs, LE), },
    c: {size:1, func:(view, offs, LE) =>
        String.fromCharCode(view.getUint8(offs)), },
    //type 's' (string) is handled specially
};

export default function Struct(...fields) {
    /** A binary structure definition.
     *  fields: The field definitions.
     */
    //keep track of field names so we can detect duplicates.
    const _normalizedFields = {};
    let structSize = 0; //total size

    let order = '>'; //big endian
    if(fields[0] == '<') {
        order = '<'; //little endian
        fields = fields.slice(1);
    }

    for(let field of fields) {
        //extract field specification
        //can be: [type, name]
        //or:     [type, name, count]
        //or:     [type, name, count, offset]
        //or:     {type:type, name:name, ...}
        //where type can be eg 'b', '4s', MyType, MyType[4]
        let name, type, offset, count, size;
        if(field instanceof Array) [type, name, count, offset] = field;
        else {
            type  = field.type;  name   = field.name;
            count = field.count; offset = field.offset;
        }
        if(_normalizedFields[name]) {
            throw new Error(`Duplicate field name: "${name}"`);
        }
        //console.log("Field", name, type, offset, count, size);

        if(typeof type == 'string') {
            //eg "I" or "4b", Python style
            //note that we can specify an array of arrays like:
            //['1024b', 'someArray', 4]
            //giving an array of 4 elements of 1024 bytes each.
            //so, don't overwrite count here.
            let tCount = type.match(/\d+/);
            tCount = (tCount == null) ? 1 : parseInt(tCount);
            type   = type.match(/\d*(.+)/)[1];
            size   = (type == 's') ? tCount : (tCount*fieldTypes[type].size);
        }
        else {
            //eg MyType or MyType[4]
            //in the latter case it resolves to {type:MyType, count:4}
            //in the former it's just MyType
            if(type.count != undefined && type.type != undefined) {
                count = type.count;
                type  = type.type;
            }
            size = type._size;
            if(size == undefined) {
                //is it a primitive type?
                if(fieldTypes[type] == undefined) {
                    console.error("lol", type);
                    throw new Error(
                        `Value ${type} cannot be used as a struct `+
                        `field (name "${name}")`);
                }
                else size = fieldTypes[type].size;
            }
        }

        if(count  == undefined) count  = 1;
        if(offset == undefined) offset = structSize;
        size *= count;
        _normalizedFields[name] = {
            //most of these fields don't need to be saved but are handy
            //to be able to reference for debugging.
            type:type, offset:offset, count:count, name:name, size:size};
        structSize = Math.max(structSize, offset+size);
    } //for
    //console.log("Fields", _normalizedFields, "size", structSize);

    const cls = class {
        [Symbol.toPrimitive](hint) {
            //called to convert this object to a primitive type
            //hint is 'number', 'string', 'default'
            if(hint == 'string') return this._toString();
            return null;
        }

        constructor(buffer, offset=0) {
            if(buffer.buffer) {
                //if given a typed array, get the underlying buffer.
                offset += buffer.byteOffset;
                buffer  = buffer.buffer;
            }
            this._buffer = buffer;
            this._offset = offset; //for debug; not used
            if(buffer.byteLength < structSize) {
                throw new Error("Buffer too small for type");
            }
            const view = new DataView(buffer);
            const LE   = (order == '<');

            //define this function inside the constructor so that it has
            //the arguments in scope
            const getVal = function(field, offs) {
                //get field value at given offset
                offs += offset;
                if(field.type == 's') { //string
                    //size is the maximum length.
                    let s = '';
                    while(s.length < field.size) {
                        const c = view.getUint8(offs);
                        if(c == 0) break;
                        offs++;
                        s += String.fromCharCode(c);
                    }
                    return s;
                }
                else if(fieldTypes[field.type]) {
                    return fieldTypes[field.type].func(view, offs, LE);
                }
                else return new field.type(buffer, offs);
            } //getVal

            //return a proxy object which will read from the given view.
            //this allows code such as:
            //let myStruct = new MyStruct(myView);
            //let x = myStruct.someField;
            return new Proxy(this, {
                get: function(instance, key) {
                    //if the class actually has this field, return that.
                    if(key in instance) res = instance[key];
                    let res;
                    const field = _normalizedFields[key];
                    if(field) {
                        //if this is one of the struct fields, read it
                        //from the view we were created with.
                        if(field.count == 1 || field.type == 's') {
                            //not an array so just return the value on its own.
                            //hey Python struct devs, are you watching?
                            return getVal(field, field.offset);
                        }
                        else {
                            //count > 1, so return an array.
                            let offs = field.offset;
                            res = [];
                            for(let i=0; i<field.count; i++) {
                                res.push(getVal(field, offs));
                                if(fieldTypes[field.type]) {
                                    offs += fieldTypes[field.type].size;
                                }
                                else offs += field.type._size;
                            }
                            return res;
                        }
                    } //if(field)
                    console.error("Field does not exist", key);
                    return undefined;
                }, //get
            }); //Proxy
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

                let offs = field.offset.toString(16).padStart(4, '0');
                res.push(`{ ${offs}: ${t} ${name}: ${items} }`);
            }
            return res.join(',\n');
        }
    }; //class

    //store the size and fields so they can be accessed outside the class.
    //these aren't used by the class itself.
    cls._size   = structSize;
    cls._fields = fields;

    //finally, return another proxy object, wrapping the class we
    //just created for this struct.
    //this is mainly so that after defining a struct type you can use
    //it in subsequent struct definitions like:
    //const StructA = Struct([ ... ]);
    //const StructB = Struct([ StructA[3], "someArray" ]);
    return new Proxy(cls, {
        get: function(instance, key) {
            if(key in instance) return instance[key];
            if(key == Symbol.toPrimitive) {
                return hint => {
                    if(hint == 'string') return "[struct]";
                    return null;
                };
            }
            //you might think checking if key is a number would be simple.
            //typeof is no good because I don't remember.
            //even parseInt can fail if given some types, hence...
            let lol;
            try { lol = isNaN(parseInt(key)); }
            catch { lol = true; }
            if(!lol) {
                //allows eg Type[n] as a struct field
                //console.log("key is int", key);
                return {type:cls, count:parseInt(key)};
            }
            //this happens when trying to check types in struct creation.
            //since this proxy represents the struct class itself, not
            //an instance, it's not really unlikely this happens.
            //console.error(`Field does not exist`, typeof key, key);
            return undefined;
        } //get
    }); //Proxy
} //function Struct
