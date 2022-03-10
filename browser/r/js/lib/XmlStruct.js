//XXX move/rename this
import { int } from "../Util.js";

function parseField(eField) {
    const result = {
        type:  eField.getAttribute('type'),
        name:  eField.getAttribute('name'),
        count: int(eField.getAttribute('count'), 1),
    };
    return result;
}

function parseXmlStruct(eStruct) {
    const result = {
        name:   eStruct.getAttribute('name'),
        fields: {},
        notes:  {},
        description: null,
    };
    let offset = 0;
    for(let child of eStruct.childNodes) {
        switch(child.tagName) {
            case 'description':
                result.description = child.textContent;
                break;

            case 'note':
                result.notes.push(child.textContent);
                break;

            case 'padding':
                offset += int(child.getAttribute('count'));
                break;

            case 'field': {
                let field = parseField(child);
                console.assert(!isNaN(field.size));
                if(result.fields[field.name]) {
                    throw new Error(`Duplicate field name: ${field.name}`);
                }
                if(field.offset == undefined) field.offset = offset;
                else offset = field.offset;
                offset += field.size;
                break;
            }

            default: throw new TypeError(`Unexpected element: ${child.tagName}`);
        }
    }
    return result;
}

export function parseXmlStructs(xml) {
    const result = {};
    const types  = xml.getElementsByTagName('types').item(0);
    for(let type of types.childNodes) {
        let obj;
        switch(type.tagName) {
            case 'struct': obj = parseXmlStruct(type); break;
            case 'enum':   obj = parseXmlEnum(type); break;
            default: throw new TypeError(`Unexpected element: ${type.tagName}`);
        }
        if(result[obj.name]) {
            throw new Error(`Duplicate type name: ${obj.name}`);
        }
    }
    return result;
}
