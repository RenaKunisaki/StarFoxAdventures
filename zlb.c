
// data is ptr to just after ZLB header
// always returns 0

// DISPLAY WARNING: Type casts are NOT being printed

int zlbDecompress(void *data,uint compLen,void *out) {
    byte bVar1;
    byte bVar2;
    ushort uVar3;
    ushort *pIn;
    ushort *puVar4;
    byte *pOut;
    uint iBit;
    byte *lengths;
    word *tbl2;
    int iVar5;
    byte *distances;
    byte *tbl4;
    int iVar6;
    uint distance2;
    uint distance;
    uint blockType;
    uint length;
    byte *tbl5;
    word *tbl6;
    byte *pbVar7;
    int iVar8;
    int iVar9;
    uint uVar10;
    uint code;
    int iVar11;
    int pbVar16;
    uint uVar12;
    bool bVar13;
    byte curByte;
    uint shift;
    uint shift2;

    pOut = out + -1;
    iBit = 0;
    shift2 = 0;
    pIn = data + 2;
    do {
        curByte = *pIn;
        shift = shift2 & 0x1f;
        distances = pIn + (iBit + 1 >> 3);
        iBit = iBit + 1 & 7;
        shift2 = 0x20 - iBit & 0x1f;
        blockType = (*distances << shift2 & 0xff | *distances >> 0x20 - shift2 |
            distances[1] << 8 - iBit) & 3;
            pIn = distances + (iBit + 2 >> 3);
            iBit = iBit + 2 & 7;
            bVar13 = iBit < 0x21;
            shift2 = 0x20 - iBit;
            if (blockType == 0) {
                // copy data to output
                if (iBit != 0) {
                    pIn = pIn + 1;
                    iBit = 0;
                }
                uVar3 = *pIn;
                puVar4 = pIn + 1;
                pIn = pIn + 2;
                length = uVar3 | *puVar4 << 8;
                do {
                    bVar1 = *pIn;
                    pIn = pIn + 1;
                    pOut = pOut + 1;
                    *pOut = bVar1;
                    blockType = bVar13;
                    bVar13 = CARRY4(length,blockType - 1);
                    length += blockType - 1;
                } while (length != 0);
            }
            else {
                if (blockType == 1) {
                    // static huffman block with standard tree
                    lengths = zlb_symbols_std;
                    tbl2 = zlb_tbl2_std;
                    iVar5 = 9;
                    distances = zlb_distances_std;
                    tbl4 = zlb_tbl4_std;
                    iVar6 = 5;
                }
                else {
                    // compressed block with table
                    lengths = zlb_symbols_custom;
                    tbl2 = zlb_tbl2_custom;
                    distances = zlb_distances_custom;
                    tbl4 = zlb_tbl4_custom;
                    iVar5 = 8;
                    tbl5 = zlb_tree1;
                    do {
                        // read the trees
                        *tbl5 = 0;
                        tbl5 = tbl5 + 1;
                        iVar5 += -1;
                    } while (iVar5 != 0);
                    iVar5 = 0x13;
                    tbl5 = zlb_tree2;
                    do {
                        *tbl5 = 0;
                        tbl5 = tbl5 + 1;
                        iVar5 += -1;
                    } while (iVar5 != 0);
                    iVar5 = 0x10;
                    tbl6 = zlb_tree3;
                    do {
                        *tbl6 = 0;
                        tbl6 = tbl6 + 1;
                        iVar5 += -1;
                    } while (iVar5 != 0);
                    iVar5 = 0x120;
                    tbl5 = lengths;
                    do {
                        *tbl5 = 0;
                        tbl5 = tbl5 + 1;
                        iVar5 += -1;
                    } while (iVar5 != 0);
                    iVar5 = 0x10;
                    tbl6 = zlb_tree4;
                    do {
                        *tbl6 = 0;
                        tbl6 = tbl6 + 1;
                        iVar5 += -1;
                    } while (iVar5 != 0);
                    iVar5 = 0x20;
                    tbl5 = distances;
                    do {
                        *tbl5 = 0;
                        tbl5 = tbl5 + 1;
                        iVar5 += -1;
                    } while (iVar5 != 0);
                    iVar6 = ((*pIn << (shift2 & 0x1f) & 0xff | *pIn >> 0x20 - (shift2 & 0x1f) |
                    *(pIn + 1) << 8 - iBit) & 0x1f) + 0x101;
                    tbl5 = pIn + (iBit + 5 >> 3);
                    iBit = iBit + 5 & 7;
                    shift2 = 0x20 - iBit & 0x1f;
                    iVar9 = ((*tbl5 << shift2 & 0xff | *tbl5 >> 0x20 - shift2 | tbl5[1] << 8 - iBit) & 0x1f) + 1
                    ;
                    tbl5 = tbl5 + (iBit + 5 >> 3);
                    uVar12 = iBit + 5 & 7;
                    bVar1 = tbl5[1];
                    bVar2 = *tbl5;
                    blockType = 0x20 - uVar12 & 0x1f;
                    iBit = uVar12 + 4;
                    pIn = tbl5 + (iBit >> 3);
                    tbl5 = zlb_order;
                    iVar5 = 0;
                    while( true ) {
                        iBit &= 7;
                        shift2 = 0x20 - iBit;
                        if (iVar5 == ((bVar2 << blockType & 0xff | bVar2 >> 0x20 - blockType | bVar1 << 8 - uVar12
                        ) & 0xf) + 4) break;
                        shift2 = (*pIn << (shift2 & 0x1f) & 0xff | *pIn >> 0x20 - (shift2 & 0x1f) |
                        *(pIn + 1) << 8 - iBit) & 7;
                        zlb_tree2[zlb_order[iVar5]] = shift2;
                        iBit += 3;
                        zlb_tree1[shift2] = zlb_tree1[shift2] + 1;
                        pIn = pIn + (iBit >> 3);
                        iVar5 += 1;
                    }
                    iVar5 = 7;
                    while (zlb_tree1[iVar5] == 0) {
                        iVar5 += -1;
                    }
                    iVar11 = 1;
                    iVar8 = 0;
                    while (iVar11 <= iVar5) {
                        if (zlb_tree1[iVar11] != 0) {
                            zlb_tbl10[iVar11] = iVar8;
                            iVar8 += zlb_tree1[iVar11] << iVar5 - iVar11;
                        }
                        iVar11 += 1;
                    }
                    iVar8 = 0;
                    while (iVar8 < 0x13) {
                        tbl5 = zlb_tbl10;
                        blockType = zlb_tree2[iVar8];
                        if (blockType != 0) {
                            iVar11 = 0;
                            while (iVar11 < 1 << iVar5 - blockType) {
                                bVar1 = zlb_tbl10[blockType];
                                zlb_tbl10[blockType] = bVar1 + 1;
                                zlb_tbl11[bVar1] = iVar8;
                                iVar11 += 1;
                            }
                        }
                        iVar8 += 1;
                    }
                    tbl6 = zlb_tree3;
                    iVar8 = 0;
                    pbVar7 = lengths;
                    do {
                        blockType = 0;
                        if (8 - iVar5 < iBit) {
                            blockType = *(pIn + 1) << 8 - iBit;
                        }
                        uVar12 = iVar5 + 0x18U & 0x1f;
                        pbVar16 = ZEXT14(zlb_tbl11[zlb_tbl12[(*pIn << (shift2 & 0x1f) & 0xff |
                        *pIn >> 0x20 - (shift2 & 0x1f) | blockType) &
                        (1 << iVar5) - 1U] << uVar12 & 0xff |
                        zlb_tbl12[(*pIn << (shift2 & 0x1f) & 0xff |
                        *pIn >> 0x20 - (shift2 & 0x1f) | blockType) &
                        (1 << iVar5) - 1U] >> 0x20 - uVar12]);
                        pIn = pIn + (iBit + zlb_tree2[pbVar16] >> 3);
                        iBit = iBit + zlb_tree2[pbVar16] & 7;
                        bVar13 = iBit < 0x21;
                        shift2 = 0x20 - iBit;
                        if (pbVar16 == 0x10) {
                            blockType = ((*pIn << (shift2 & 0x1f) & 0xff | *pIn >> 0x20 - (shift2 & 0x1f) |
                            *(pIn + 1) << 8 - iBit) & 3) + 3;
                            pIn = pIn + (iBit + 2 >> 3);
                            iBit = iBit + 2 & 7;
                            bVar13 = iBit < 0x21;
                            shift2 = 0x20 - iBit;
                            pbVar16 = tbl5;
                        }
                        else {
                            if (pbVar16 == 0x11) {
                                pbVar16 = 0;
                                blockType = ((*pIn << (shift2 & 0x1f) & 0xff | *pIn >> 0x20 - (shift2 & 0x1f) |
                                *(pIn + 1) << 8 - iBit) & 7) + 3;
                                pIn = pIn + (iBit + 3 >> 3);
                                iBit = iBit + 3 & 7;
                                bVar13 = iBit < 0x21;
                                shift2 = 0x20 - iBit;
                            }
                            else {
                                if (pbVar16 == 0x12) {
                                    pbVar16 = 0;
                                    blockType = ((*pIn << (shift2 & 0x1f) & 0xff | *pIn >> 0x20 - (shift2 & 0x1f) |
                                    *(pIn + 1) << 8 - iBit) & 0x7f) + 0xb;
                                    pIn = pIn + (iBit + 7 >> 3);
                                    iBit = iBit + 7 & 7;
                                    bVar13 = iBit < 0x21;
                                    shift2 = 0x20 - iBit;
                                }
                                else {
                                    blockType = 1;
                                }
                            }
                        }
                        do {
                            pbVar7[iVar8] = pbVar16;
                            iVar8 += 1;
                            tbl6[pbVar16] = tbl6[pbVar16] + 1;
                            if ((pbVar7 == zlb_symbols_custom) && (iVar8 == iVar6)) {
                                tbl6 = zlb_tree4;
                                iVar8 = 0;
                                pbVar7 = distances;
                            }
                            uVar12 = bVar13;
                            bVar13 = CARRY4(blockType,uVar12 - 1);
                            blockType += uVar12 - 1;
                        } while (blockType != 0);
                        tbl5 = pbVar16;
                    } while ((pbVar7 == zlb_symbols_custom) || (iVar8 < iVar9));
                    iVar5 = 0xf;
                    tbl6 = zlb_tree3 + 0xf;
                    while (*tbl6 == 0) {
                        tbl6 = tbl6 + -1;
                        iVar5 += -1;
                    }
                    iVar11 = 1;
                    iVar8 = 0;
                    while (iVar11 <= iVar5) {
                        if (zlb_tree3[iVar11] != 0) {
                            zlb_tbl13[iVar11] = iVar8;
                            iVar8 += zlb_tree3[iVar11] << iVar5 - iVar11;
                        }
                        iVar11 += 1;
                    }
                    iVar8 = 0;
                    while (iVar8 < iVar6) {
                        distance2 = zlb_symbols_custom[iVar8];
                        if (distance2 != 0) {
                            iVar11 = 0;
                            while (iVar11 < 1 << iVar5 - distance2) {
                                uVar3 = zlb_tbl13[distance2];
                                zlb_tbl13[distance2] = uVar3 + 1;
                                zlb_tbl2_custom[uVar3] = iVar8;
                                iVar11 += 1;
                            }
                        }
                        iVar8 += 1;
                    }
                    iVar6 = 0xf;
                    while (zlb_tree4[iVar6] == 0) {
                        iVar6 += -1;
                    }
                    iVar11 = 1;
                    iVar8 = 0;
                    while (iVar11 <= iVar6) {
                        if (zlb_tree4[iVar11] != 0) {
                            zlb_tbl14[iVar11] = iVar8;
                            iVar8 += zlb_tree4[iVar11] << iVar6 - iVar11;
                        }
                        iVar11 += 1;
                    }
                    iVar8 = 0;
                    while (iVar8 < iVar9) {
                        distance = zlb_distances_custom[iVar8];
                        if (distance != 0) {
                            iVar11 = 0;
                            while (iVar11 < 1 << iVar6 - distance) {
                                uVar3 = zlb_tbl14[distance];
                                zlb_tbl14[distance] = uVar3 + 1;
                                zlb_tbl4_custom[uVar3] = iVar8;
                                iVar11 += 1;
                            }
                        }
                        iVar8 += 1;
                    }
                }
                do {
                    uVar10 = (*pIn << (shift2 & 0x1f) & 0xff | *pIn >> 0x20 - (shift2 & 0x1f) |
                    *(pIn + 1) << 8 - iBit | *(pIn + 1) << 0x10 - iBit) & (1 << iVar5) - 1U;
                    blockType = iVar5 - 8U & 0x1f;
                    uVar12 = iVar5 + 0x10U & 0x1f;
                    code = tbl2[zlb_tbl12[uVar10 & 0xff] << blockType & 0xffff |
                    zlb_tbl12[uVar10 & 0xff] >> 0x20 - blockType |
                    zlb_tbl12[uVar10 >> 8] << uVar12 & 0xff |
                    zlb_tbl12[uVar10 >> 8] >> 0x20 - uVar12];
                    pIn = pIn + (iBit + lengths[code] >> 3);
                    iBit = iBit + lengths[code] & 7;
                    shift2 = 0x20 - iBit;
                    if (code < 0x100) {
                        pOut = pOut + 1;
                        *pOut = tbl2[zlb_tbl12[uVar10 & 0xff] << blockType & 0xffff |
                        zlb_tbl12[uVar10 & 0xff] >> 0x20 - blockType |
                        zlb_tbl12[uVar10 >> 8] << uVar12 & 0xff |
                        zlb_tbl12[uVar10 >> 8] >> 0x20 - uVar12];
                    }
                    else {
                        if (code == 0x100) break;
                        uVar12 = zlb_lbase[(code - 0x101) * 2];
                        blockType = zlb_lbase[(code - 0x101) * 2 + 1];
                        if (blockType != 0) {
                            uVar12 += (*pIn << (shift2 & 0x1f) & 0xff | *pIn >> 0x20 - (shift2 & 0x1f) |
                            *(pIn + 1) << 8 - iBit) & (1 << blockType) - 1U;
                            pIn = pIn + (iBit + blockType >> 3);
                            iBit = iBit + blockType & 7;
                            shift2 = 0x20 - iBit;
                        }
                        uVar10 = (*pIn << (shift2 & 0x1f) & 0xff | *pIn >> 0x20 - (shift2 & 0x1f) |
                        *(pIn + 1) << 8 - iBit | *(pIn + 1) << 0x10 - iBit) & (1 << iVar6) - 1U;
                        shift2 = iVar6 - 8U & 0x1f;
                        blockType = iVar6 + 0x10U & 0x1f;
                        blockType = tbl4[zlb_tbl12[uVar10 & 0xff] << shift2 & 0xffff |
                        zlb_tbl12[uVar10 & 0xff] >> 0x20 - shift2 |
                        zlb_tbl12[uVar10 >> 8] << blockType & 0xff |
                        zlb_tbl12[uVar10 >> 8] >> 0x20 - blockType];
                        pIn = pIn + (iBit + distances[blockType] >> 3);
                        iBit = iBit + distances[blockType] & 7;
                        shift2 = 0x20 - iBit;
                        uVar10 = zlb_dbase[blockType * 2];
                        blockType = zlb_dbase[blockType * 2 + 1];
                        if (blockType != 0) {
                            uVar10 += (*pIn << (shift2 & 0x1f) & 0xff | *pIn >> 0x20 - (shift2 & 0x1f) |
                            *(pIn + 1) << 8 - iBit | *(pIn + 1) << 0x10 - iBit) & (1 << blockType) - 1U;
                            pIn = pIn + (iBit + blockType >> 3);
                            iBit = iBit + blockType & 7;
                            shift2 = 0x20 - iBit;
                        }
                        tbl5 = pOut + -uVar10;
                        do {
                            tbl5 = tbl5 + 1;
                            pOut = pOut + 1;
                            *pOut = *tbl5;
                            uVar12 -= 1;
                        } while (uVar12 != 0);
                    }
                } while (code != 0x100);
            }
            if (((curByte << shift | curByte >> 0x20 - shift) & 1) != 0) {
                return 0;
            }
        } while( true );
    }
