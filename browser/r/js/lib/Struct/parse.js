import { getXml, hex, int } from "../../Util.js";
import Struct from "./Struct.js";
import Field from "./Field.js";
import { types } from "./Type.js";

function parseField(eField, offset, littleEndian=false) {
    /** Parse one 'field' element. */
    let offs = eField.getAttribute('offset');
    if(offs == undefined) offs = offset;
    const tName = eField.getAttribute('type');
    const type  = types[tName];
    if(type == undefined) throw new TypeError(`Unknown type: '${tName}'`);

    let name = eField.getAttribute('name');
    if(!name) name = `_${hex(offset,2)}`;

    const order = eField.getAttribute('order');
    switch(order) {
        case undefined: case null: littleEndian = false; break;
        case '>': littleEndian = false; break;
        case '<': littleEndian = true; break;
        default: throw new Error(`Invalid value '${order}' for attribute 'order'`);
    }

    return new Field(offset, new type(), name, littleEndian,
        int(eField.getAttribute('count'), 1));
}

function parseXmlStruct(eStruct) {
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
        if(child.tagName == undefined) continue;
        switch(child.tagName) {
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
                const field = parseField(child, offset, result.littleEndian);
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

const parsers = {
    struct: parseXmlStruct,
    //enum: parseXmlEnum,
    //typedef: parseXmlTypedef,
};

export function parseXmlStructs(xml) {
    /** Parse an XML structs document. */
    const result = {};
    const types  = xml.getElementsByTagName('types').item(0);
    for(let type of types.childNodes) {
        if(type.tagName == undefined) continue;
        const parser = parsers[type.tagName];
        if(!parser) throw new TypeError(`Unexpected element: ${type.tagName}`);
        const obj = parser(type);
        if(result[obj.name]) {
            throw new Error(`Duplicate type name: ${obj.name}`);
        }
        result[obj.name] = obj;
    }
    return result;
}

export default async function getStructs(path) {
    const xml = await getXml(path);
    return parseXmlStructs(xml);
}
