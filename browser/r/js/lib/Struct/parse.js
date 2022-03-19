import { getXml, hex, int } from "../../Util.js";
import Struct from "./Struct.js";
import Field from "./Field.js";
import { Type, types } from "./Type.js";

export default class StructParser {
    constructor() {
        this.types = {};
        this._parsedPaths = new Set();
    }

    _parseField(eField, offset, littleEndian=false) {
        /** Parse one 'field' element. */
        const field = {
            offset: offset,
            littleEndian: littleEndian,
            count: int(eField.getAttribute('count'), 1),
            description: null,
            notes: [],
        };

        let offs = eField.getAttribute('offset');
        if(offs == undefined) offs = offset;
        const tName = eField.getAttribute('type');
        const type  = types[tName];
        if(type == undefined) throw new TypeError(`Unknown type: '${tName}'`);
        field.type = new type();

        let name = eField.getAttribute('name');
        if(!name) name = `_${hex(offset,2)}`;
        field.name = name;

        const order = eField.getAttribute('order');
        switch(order) {
            case undefined: case null:
                field.littleEndian = false; break;
            case '>': field.littleEndian = false; break;
            case '<': field.littleEndian = true; break;
            default: throw new Error(
                `Invalid value '${order}' for attribute 'order'`);
        }

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

    _parseStruct(eStruct) {
        /** Parse one 'struct' element. */
        const result = {
            name:         eStruct.getAttribute('name'),
            fields:       [],
            fieldsByName: {},
            notes:        [],
            description:  null,
            littleEndian: false,
        };
        //if(!result.name) throw new Error("No name specified for struct");

        const order = eStruct.getAttribute('order');
        switch(order) {
            case undefined: case null: result.littleEndian = false; break;
            case '>': result.littleEndian = false; break;
            case '<': result.littleEndian = true; break;
            default: throw new Error(`Invalid value '${order}' for attribute 'order'`);
        }

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
                        child, offset, result.littleEndian);
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
            struct: elem => this._parseStruct(elem),
            //enum: parseXmlEnum,
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
            if(namespace[nsName] == undefined) namespace[nsName] = {};
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
