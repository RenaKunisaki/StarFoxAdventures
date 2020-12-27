for(int iy=0; iy < entry->height; iy++) {
    for(int ix=0; ix < entry->width; ix++) {
        c = ix + iy * entry->width;
        //b = highest byte
        b = (entry->field_C[c] >> 23) & 0xFF;
        if (b != 0xff) {
            out[c >> 7] |= 1 << (c & 3);
        }
    }
}
