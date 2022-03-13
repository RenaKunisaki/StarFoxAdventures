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
