if (id < 0) id2 = -id;
else if ((id < 3000) || ((ushort)pTexTableBin[id] == 0)) {
    id2 = (uint)(ushort)pTexTableBin[id];
}
else id2 = (ushort)pTexTableBin[id] + 1;

id3 = id2 & 0xffff;
if ((id2 & 0x8000) == 0) {
    if (id < 3000) {
        tblIdx = 0;
        texFile = TEX0_bin;
    }
    else {
        tblIdx = 2;
        texFile = TEXPRE_bin;
    }
}
else {
    tblIdx = 1;
    texFile = TEX1_bin;
    id3 = id2 & 0x7fff;
}

/* 0C15 => 0001, 0C16 => 0002, makes sense as
these are the debug fonts. but the game actually
loads them using IDs 0001, 0002. so is this whole
TEXTABLE thing backward?
TEXTABLE[0xC15] == 1
but it doesn't make sense that this would be used
as a lookup, then...
but also TEXTABLE[1] == 1
0xC15 = 3093
so we did have the wrong IDs for unpacking

does the game use 0xC15 at all? maybe this translation
isn't actually used in practice. (the game looks it up
but maybe all used IDs just map to themselves?)

if we hack it to load 0xC15 instead of 1 for the debug
font it loads the wrong thing, even though entry 0xC15
in TEXTABLE is 1. probably because 0xC15 > 3000?

if we just skip the translation entirely it looks right.
1 is the debug font, 725 is Krystal as usual
but if we replace TEXTABLE.bin with one that's all zeros
a lot of textures are wrong, so something is happening

debug font is in TEX0, Krystal is TEX1
in theory the high bit of TEXTABLE tells which file to
use but it looks like none are ever actually set.

Krystal model specifies textures 0xD4..0xD9. how do those
translate to 724..?
*/

//in modelLoad:
for(iTex = 0; iTex < (int)(uint)header2->nTextures; iTex++) {
    tex = (uint)textureLoad(
        -(header2->textures[iTex] | 0x8000),true);
    header2->textures[iTex] = tex;
}
//so model textures are always in TEX1 and not translated
/* so maybe there is no consistent way to translate index to ID?

should change the program to only pack/unpack one texture
file like older ones did, so you'd have animtest/TEX0/####.png, etc
*/
