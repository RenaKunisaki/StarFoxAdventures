import BytesIO from "./BytesIO.js";

export function swizzle_3_bit_to_8_bit(v) {
    //00000123 -> 12312312
    return (v << 5) | (v << 2) | (v >> 1);
}
export function swizzle_4_bit_to_8_bit(v) {
    //00001234 -> 12341234
    return (v << 4) | (v >> 0);
}
export function swizzle_5_bit_to_8_bit(v) {
    //00012345 -> 12345123
    return (v << 3) | (v >> 2);
}
export function swizzle_6_bit_to_8_bit(v) {
    //00123456 -> 12345612
    return (v << 2) | (v >> 4);
}
export function read_u8(data, offset) {
    if(data instanceof BytesIO) return data.read_u8(offset);
    if(data instanceof DataView) return data.getUint8(offset);
    return data[offset];
}
export function read_u16(data, offset) {
    if(data instanceof BytesIO) return data.read_u16(offset);
    if(data instanceof DataView) return data.getUint16(offset);
    return (data[offset] << 8) | data[offset+1];
}
export function read_u32(data, offset) {
    if(data instanceof BytesIO) return data.read_u32(offset);
    if(data instanceof DataView) return data.getUint32(offset);
    return (
        (data[offset  ] << 24) |
        (data[offset+1] << 16) |
        (data[offset+2] <<  8) |
         data[offset+3]);
}
export function write_bytes(data, offset, raw_bytes) {
    data.seek(offset);
    data.write(raw_bytes);
}
export function write_u8(data, offset, new_value) {
    new_value = struct.pack(">B", new_value);
    data.seek(offset);
    data.write(new_value);
}
export function write_u16(data, offset, new_value) {
    new_value = struct.pack(">H", new_value);
    data.seek(offset);
    data.write(new_value);
}
export function write_u32(data, offset, new_value) {
    new_value = struct.pack(">I", new_value);
    data.seek(offset);
    data.write(new_value);
}
