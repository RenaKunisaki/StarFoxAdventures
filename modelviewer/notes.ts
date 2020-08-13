vat[0][GX.Attr.POS] = { compType: GX.CompType.S16, compShift: 0, compCnt: GX.CompCnt.POS_XYZ };
vat[0][GX.Attr.CLR0] = { compType: GX.CompType.RGBA8, compShift: 0, compCnt: GX.CompCnt.CLR_RGBA };
vat[0][GX.Attr.TEX0] = { compType: GX.CompType.S16, compShift: 7, compCnt: GX.CompCnt.TEX_ST };

vat[1][GX.Attr.POS] = { compType: GX.CompType.S16, compShift: 2, compCnt: GX.CompCnt.POS_XYZ };
vat[1][GX.Attr.CLR0] = { compType: GX.CompType.RGBA8, compShift: 0, compCnt: GX.CompCnt.CLR_RGBA };
vat[1][GX.Attr.TEX0] = { compType: GX.CompType.F32, compShift: 0, compCnt: GX.CompCnt.TEX_ST };

vat[2][GX.Attr.POS] = { compType: GX.CompType.F32, compShift: 0, compCnt: GX.CompCnt.POS_XYZ };
vat[2][GX.Attr.NRM] = { compType: GX.CompType.F32, compShift: 0, compCnt: GX.CompCnt.NRM_XYZ };
vat[2][GX.Attr.CLR0] = { compType: GX.CompType.RGBA8, compShift: 0, compCnt: GX.CompCnt.CLR_RGBA };
vat[2][GX.Attr.TEX0] = { compType: GX.CompType.F32, compShift: 0, compCnt: GX.CompCnt.TEX_ST };
vat[2][GX.Attr.TEX1] = { compType: GX.CompType.F32, compShift: 0, compCnt: GX.CompCnt.TEX_ST };

vat[3][GX.Attr.POS] = { compType: GX.CompType.S16, compShift: 8, compCnt: GX.CompCnt.POS_XYZ };
vat[3][GX.Attr.NRM] = { compType: GX.CompType.S8, compShift: 0, compCnt: GX.CompCnt.NRM_XYZ };
vat[3][GX.Attr.CLR0] = { compType: GX.CompType.RGBA4, compShift: 0, compCnt: GX.CompCnt.CLR_RGBA };
vat[3][GX.Attr.TEX0] = { compType: GX.CompType.S16, compShift: 10, compCnt: GX.CompCnt.TEX_ST };
vat[3][GX.Attr.TEX1] = { compType: GX.CompType.S16, compShift: 10, compCnt: GX.CompCnt.TEX_ST };
vat[3][GX.Attr.TEX2] = { compType: GX.CompType.S16, compShift: 10, compCnt: GX.CompCnt.TEX_ST };
vat[3][GX.Attr.TEX3] = { compType: GX.CompType.S16, compShift: 10, compCnt: GX.CompCnt.TEX_ST };

vat[4][GX.Attr.POS] = { compType: GX.CompType.F32, compShift: 0, compCnt: GX.CompCnt.POS_XYZ };
vat[4][GX.Attr.NRM] = { compType: GX.CompType.F32, compShift: 0, compCnt: GX.CompCnt.NRM_XYZ };
vat[4][GX.Attr.CLR0] = { compType: GX.CompType.RGBA8, compShift: 0, compCnt: GX.CompCnt.CLR_RGBA };
vat[4][GX.Attr.TEX0] = { compType: GX.CompType.S16, compShift: 7, compCnt: GX.CompCnt.TEX_ST };

vat[5][GX.Attr.POS] = { compType: GX.CompType.S16, compShift: fields.oldVat ? 0 : 3, compCnt: GX.CompCnt.POS_XYZ };
vat[5][GX.Attr.NRM] = { compType: GX.CompType.S8, compShift: 0, compCnt: GX.CompCnt.NRM_XYZ };
vat[5][GX.Attr.CLR0] = { compType: GX.CompType.RGBA4, compShift: 0, compCnt: GX.CompCnt.CLR_RGBA };
vat[5][GX.Attr.TEX0] = { compType: GX.CompType.S16, compShift: 8, compCnt: GX.CompCnt.TEX_ST };
vat[5][GX.Attr.TEX1] = { compType: GX.CompType.S16, compShift: 8, compCnt: GX.CompCnt.TEX_ST };
vat[5][GX.Attr.TEX2] = { compType: GX.CompType.S16, compShift: 8, compCnt: GX.CompCnt.TEX_ST };
vat[5][GX.Attr.TEX3] = { compType: GX.CompType.S16, compShift: 8, compCnt: GX.CompCnt.TEX_ST };

vat[6][GX.Attr.POS] = { compType: GX.CompType.S16, compShift: 8, compCnt: GX.CompCnt.POS_XYZ };
vat[6][GX.Attr.NRM] = { compType: GX.CompType.S8, compShift: 0, compCnt: GX.CompCnt.NRM_XYZ };
vat[6][GX.Attr.CLR0] = { compType: GX.CompType.RGBA4, compShift: 0, compCnt: GX.CompCnt.CLR_RGBA };
vat[6][GX.Attr.TEX0] = { compType: GX.CompType.S16, compShift: 10, compCnt: GX.CompCnt.TEX_ST };
vat[6][GX.Attr.TEX1] = { compType: GX.CompType.S16, compShift: 10, compCnt: GX.CompCnt.TEX_ST };
vat[6][GX.Attr.TEX2] = { compType: GX.CompType.S16, compShift: 10, compCnt: GX.CompCnt.TEX_ST };
vat[6][GX.Attr.TEX3] = { compType: GX.CompType.S16, compShift: 10, compCnt: GX.CompCnt.TEX_ST };

vat[7][GX.Attr.POS] = { compType: GX.CompType.S16, compShift: 0, compCnt: GX.CompCnt.POS_XYZ };
vat[7][GX.Attr.NRM] = { compType: GX.CompType.S8, compShift: 0, compCnt: GX.CompCnt.NRM_XYZ };
vat[7][GX.Attr.CLR0] = { compType: GX.CompType.RGBA4, compShift: 0, compCnt: GX.CompCnt.CLR_RGBA };
vat[7][GX.Attr.TEX0] = { compType: GX.CompType.S16, compShift: 10, compCnt: GX.CompCnt.TEX_ST };
vat[7][GX.Attr.TEX1] = { compType: GX.CompType.S16, compShift: 10, compCnt: GX.CompCnt.TEX_ST };
vat[7][GX.Attr.TEX2] = { compType: GX.CompType.S16, compShift: 10, compCnt: GX.CompCnt.TEX_ST };
vat[7][GX.Attr.TEX3] = { compType: GX.CompType.S16, compShift: 10, compCnt: GX.CompCnt.TEX_ST };

export const enum Attr {
    PNMTXIDX = 0,
    TEX0MTXIDX = 1,
    TEX1MTXIDX = 2,
    TEX2MTXIDX = 3,
    TEX3MTXIDX = 4,
    TEX4MTXIDX = 5,
    TEX5MTXIDX = 6,
    TEX6MTXIDX = 7,
    TEX7MTXIDX = 8,
    POS = 9,
    NRM = 10,
    CLR0 = 11,
    CLR1 = 12,
    TEX0 = 13,
    TEX1 = 14,
    TEX2 = 15,
    TEX3 = 16,
    TEX4 = 17,
    TEX5 = 18,
    TEX6 = 19,
    TEX7 = 20,
    MAX = TEX7,
    NBT = 25,
    NULL = 0xFF,
}

export const enum CompCnt {
    // Position
    POS_XY = 0,
    POS_XYZ = 1,
    // Normal
    NRM_XYZ = 0,
    NRM_NBT = 1,
    NRM_NBT3 = 2,
    // Color
    CLR_RGB = 0,
    CLR_RGBA = 1,
    // TexCoord
    TEX_S = 0,
    TEX_ST = 1,
}

export const enum CompType {
    U8 = 0,
    S8 = 1,
    U16 = 2,
    S16 = 3,
    F32 = 4,

    RGB565 = 0,
    RGB8 = 1,
    RGBX8 = 2,
    RGBA4 = 3,
    RGBA6 = 4,
    RGBA8 = 5,
}

export const enum CompareType {
    NEVER = 0,
    LESS = 1,
    EQUAL = 2,
    LEQUAL = 3,
    GREATER = 4,
    NEQUAL = 5,
    GEQUAL = 6,
    ALWAYS = 7,
}

function readVertexDesc(bits: LowBitReader, shader: Shader): GX_VtxDesc[] {
    // console.log(`Setting descriptor`);
    const vcd: GX_VtxDesc[] = [];
    for (let i = 0; i <= GX.Attr.MAX; i++) {
        vcd[i] = { type: GX.AttrType.NONE };
    }
    vcd[GX.Attr.NBT] = { type: GX.AttrType.NONE };

    if (fields.hasBones && jointCount >= 2) {
        vcd[GX.Attr.PNMTXIDX].type = GX.AttrType.DIRECT;

        let texmtxNum = 0;

        if (shader.hasAuxTex0 || shader.hasAuxTex1) {
            if (shader.hasAuxTex2) {
                vcd[GX.Attr.TEX0MTXIDX + texmtxNum].type = GX.AttrType.DIRECT;
                texmtxNum++;
                vcd[GX.Attr.TEX0MTXIDX + texmtxNum].type = GX.AttrType.DIRECT;
                texmtxNum++;
            }

            vcd[GX.Attr.TEX0MTXIDX + texmtxNum].type = GX.AttrType.DIRECT;
            texmtxNum++;
        }

        texmtxNum = 7;
        for (let i = 0; i < texMtxCount; i++) {
            vcd[GX.Attr.TEX0MTXIDX + texmtxNum].type = GX.AttrType.DIRECT;
            texmtxNum--;
        }
    }

    const posDesc = bits.get(1);
    vcd[GX.Attr.POS].type = posDesc ? GX.AttrType.INDEX16 : GX.AttrType.INDEX8;

    if (fields.hasNormals && (shader.attrFlags & ShaderAttrFlags.NRM)) {
        const nrmDesc = bits.get(1);
        if (nrmTypeFlags & 8) {
            // TODO: Enable NBT normals
            // vcd[GX.Attr.NRM].type = GX.AttrType.NONE;
            // vcd[GX.Attr.NBT].type = nrmDesc ? GX.AttrType.INDEX16 : GX.AttrType.INDEX8;
            vcd[GX.Attr.NRM].type = nrmDesc ? GX.AttrType.INDEX16 : GX.AttrType.INDEX8;
        } else {
            // vcd[GX.Attr.NBT].type = GX.AttrType.NONE;
            vcd[GX.Attr.NRM].type = nrmDesc ? GX.AttrType.INDEX16 : GX.AttrType.INDEX8;
        }
    } else {
        vcd[GX.Attr.NRM].type = GX.AttrType.NONE;
    }

    if (shader.attrFlags & ShaderAttrFlags.CLR) {
        const clr0Desc = bits.get(1);
        vcd[GX.Attr.CLR0].type = clr0Desc ? GX.AttrType.INDEX16 : GX.AttrType.INDEX8;
    } else {
        vcd[GX.Attr.CLR0].type = GX.AttrType.NONE;
    }

    const texCoordDesc = bits.get(1);
    if (shader.layers.length > 0) {
        // Note: texCoordDesc applies to all texture coordinates in the vertex
        for (let t = 0; t < 8; t++) {
            if (t < shader.layers.length) {
                vcd[GX.Attr.TEX0 + t].type = texCoordDesc ? GX.AttrType.INDEX16 : GX.AttrType.INDEX8;
            } else {
                vcd[GX.Attr.TEX0 + t].type = GX.AttrType.NONE;
            }
        }
    }

    return vcd;
}
