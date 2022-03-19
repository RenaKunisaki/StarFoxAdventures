import { getXml, hex, int } from "../../Util.js";
import Struct from "./Struct.js";
import Field from "./Field.js";
import { EnumItem, Enum } from "./Enum.js";
import { Type, types } from "./Type.js";

export default class StructParser {
    constructor() {
        this._parsedPaths = new Set();
        this.types = {};
        this._namespaceParents = {}; //namespace => parent
        for(let [name, tp] of Object.entries(types)) {
            this.types[name] = new tp();
        }
    }

    getType(name, namespace=null) {
        if(namespace == null) namespace = this.types;
        while(namespace != undefined) {
            let result = namespace;
            for(let n of name.split('.')) {
                result = result[n];
            }
            if(result != undefined) return result;
            namespace = this._namespaceParents[namespace];
        }
        console.error("Type not found", name);
    }

    _parseEnumItem(eItem, namespace=null) {
        /** Parse one 'item' element in an enum. */
        if(namespace == null) namespace = this.types;
        const result = {
            name:         eItem.getAttribute('name'),
            value:        int(eItem.getAttribute('value')),
            description:  null,
            notes:        [],
        };
        for(let child of eItem.childNodes) {
            switch(child.tagName) {
                case undefined: continue; //text node
                case 'description':
                    if(result.description != null) {
                        throw new Error("Multiple 'description' elements");
                    }
                    result.description = child.textContent;
                    break;

                case 'note':
                    result.notes.push(child.textContent);
                    break;

                default: throw new TypeError(
                    `Unexpected element: ${child.tagName}`);
            }
        }
        return new EnumItem(result);
    }

    _parseEnum(eEnum, namespace=null) {
        /** Parse one 'enum' element. */
        if(namespace == null) namespace = this.types;
        const result = {
            name:         eEnum.getAttribute('name'),
            type:         types.int,
            fields:       {},
            fieldsByName: {},
            description:  null,
            notes:        [],
        };

        //get type
        let tName = eEnum.getAttribute('type');
        if(tName != null && tName != undefined) {
            const type = this.getType(tName, namespace);
            if(type) result.type = type;
            else throw new Error(`Unknown type: '${tName}'`);
        }

        //get items
        let value = 0;
        for(let child of eEnum.childNodes) {
            switch(child.tagName) {
                case undefined: continue; //text node
                case 'item': {
                    let item = this._parseEnumItem(child, namespace);
                    if(item.value != undefined && item.value != null) {
                        value = item.value;
                    }
                    item.value = value++;
                    if(result.fields[item.value] != undefined) {
                        throw new Error(
                            `Multiple instances of value '${item.value}' in enum '${result.name}'`);
                    }
                    result.fields[item.value] = item;

                    if(result.fieldsByName[item.name] != undefined) {
                        throw new Error(
                            `Multiple instances of name '${item.name}' in enum '${result.name}'`);
                    }
                    result.fieldsByName[item.name] = item;
                    break;
                }

                case 'description':
                    if(result.description != null) {
                        throw new Error("Multiple 'description' elements");
                    }
                    result.description = child.textContent;
                    break;

                case 'note':
                    result.notes.push(child.textContent);
                    break;

                default: throw new TypeError(
                    `Unexpected element: ${child.tagName}`);
            }
        }
        return new Enum(result);
    }

    _parseField(eField, offset, namespace=null, littleEndian=false) {
        /** Parse one 'field' element. */
        if(namespace == null) namespace = this.types;
        const field = {
            offset: offset,
            littleEndian: littleEndian,
            count: int(eField.getAttribute('count'), 1),
            description: null,
            notes: [],
        };

        //get offset
        let offs = eField.getAttribute('offset');
        if(offs == undefined) offs = offset;

        //get type
        const tName = eField.getAttribute('type');
        const type  = this.getType(tName, namespace);
        if(type == undefined) throw new TypeError(`Unknown type: '${tName}'`);
        field.type = type;

        //get name
        let name = eField.getAttribute('name');
        if(!name) name = `_${hex(offset,2)}`;
        field.name = name;

        //get byte order
        const order = eField.getAttribute('order');
        switch(order) {
            case undefined: case null:
                field.littleEndian = false; break;
            case '>': field.littleEndian = false; break;
            case '<': field.littleEndian = true; break;
            default: throw new Error(
                `Invalid value '${order}' for attribute 'order'`);
        }

        //get notes
        for(let child of eField.childNodes) {
            switch(child.tagName) {
                case undefined: continue; //text node
                case 'description':
                    if(field.description != null) {
                        throw new Error("Multiple 'description' elements");
                    }
                    field.description = child.textContent;
                    break;

                case 'note':
                    field.notes.push(child.textContent);
                    break;

                default: throw new TypeError(
                    `Unexpected element: ${child.tagName}`);
            }
        }

        return new Field(field);
    }

    _parseStruct(eStruct, namespace) {
        /** Parse one 'struct' element. */
        if(namespace == null) namespace = this.types;
        const result = {
            name:         eStruct.getAttribute('name'),
            fields:       [],
            fieldsByName: {},
            notes:        [],
            description:  null,
            littleEndian: false,
        };
        //if(!result.name) throw new Error("No name specified for struct");

        //get byte order
        const order = eStruct.getAttribute('order');
        switch(order) {
            case undefined: case null: result.littleEndian = false; break;
            case '>': result.littleEndian = false; break;
            case '<': result.littleEndian = true; break;
            default: throw new Error(`Invalid value '${order}' for attribute 'order'`);
        }

        //get elements
        let offset = 0;
        for(let child of eStruct.childNodes) {
            switch(child.tagName) {
                case undefined: continue; //text node
                case 'description':
                    if(result.description != null) {
                        throw new Error("Multiple 'description' elements");
                    }
                    result.description = child.textContent;
                    break;

                case 'note':
                    result.notes.push(child.textContent);
                    break;

                case 'padding':
                    offset += int(child.getAttribute('count'));
                    break;

                case 'field': {
                    const field = this._parseField(
                        child, offset, namespace, result.littleEndian);
                    console.assert(!isNaN(field.size));
                    if(result.fieldsByName[field.name]) {
                        throw new Error(`Duplicate field name: ${field.name}`);
                    }
                    result.fieldsByName[field.name] = field;
                    result.fields.push(field);
                    offset = field.offset + field.size;
                    break;
                }

                default: throw new TypeError(`Unexpected element: ${child.tagName}`);
            }
        }
        return new Struct(result);
    }

    async parseFile(path, namespace=null) {
        /** Parse an XML structs document. */
        const parsers = {
            struct: elem => this._parseStruct(elem, namespace),
            enum:   elem => this._parseEnum(elem, namespace),
            //typedef: parseXmlTypedef,
        };

        console.log("Parsing", path);
        this._parsedPaths.add(path);
        const xml   = await getXml(path);
        const types = xml.getElementsByTagName('types').item(0);
        const nsName = types.getAttribute('namespace');
        if(namespace == null) namespace = this.types;
        if(nsName != null && nsName != undefined) {
            if(nsName == '') throw new Error("Empty namespace name");
            if(namespace[nsName] == undefined) {
                const newNs = {};
                namespace[nsName] = newNs;
                this._namespaceParents[newNs] = namespace;
            }
            namespace = namespace[nsName];
        }

        for(let type of types.childNodes) {
            if(type.tagName == undefined) continue;
            else if(type.tagName == 'include') {
                let dir = path.split('/');
                dir.pop(); //remove name
                dir.push(type.getAttribute('path'));
                dir = dir.join('/');
                if(!this._parsedPaths.has(dir)) {
                    await this.parseFile(dir, namespace);
                }
            }
            else {
                const parser = parsers[type.tagName];
                if(!parser) {
                    throw new TypeError(`Unexpected element: ${type.tagName}`);
                }
                const obj = parser(type);
                if(namespace[obj.name]) {
                    throw new Error(`Duplicate type name: ${obj.name}`);
                }
                namespace[obj.name] = obj;
            }
        }
        return namespace;
    }

    async selfTest() {
        const structs = await this.parseFile('/data/types/common.xml');
        console.log("structs:", structs);
        const testBuffer = new ArrayBuffer(64);
        const testView = new DataView(testBuffer);
        structs.vec3f.write({x:1, y:2, z:3}, testView);
        const testData = structs.vec3f.read(testView);
        console.assert(testData.x == 1);
        console.assert(testData.y == 2);
        console.assert(testData.z == 3);
    }
}
