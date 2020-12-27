
// WARNING: Removing unreachable block (ram,0x80007510)
// WARNING: Removing unreachable block (ram,0x80007500)
// WARNING: Removing unreachable block (ram,0x800074fc)

// DISPLAY WARNING: Type casts are NOT being printed

void animInterpretAnimationData(
    float      *param_1,     //r3
    Mtx44      *mtxs,        //r4
    undefined  *param_3,     //r5
    AnimHeader *animData,    //r6
    undefined4  nEntries,    //r7
    ushort     *param_6,     //r8
    undefined4  boneIdxMask, //r9
    undefined4  flags,       //r10
    vec3s      *vecs,        //r11
    uint       *param_10,    //r12 - lengths?
    double      param_11,    //f4
    int         _r20,        //r20
    vec3s      *pOut,        //r31
    undefined4  param_14,    //stack 0xC
) {

    ushort     *next;
    uint        entryLo;
    ushort      entryHi;
    short      *psVar4;
    ushort      entry;
    uint        uVar5;
    uint        uVar7;
    bool        in_xer_ca;
    bool        bVar8;
    undefined4  uVar9;

    uint       *pData    = _r20 + param_10;
    uint        uVar1    = param_11 - (param_14 * 16384.0);
    uint        uVar2    = animData->field_0x0 * 3;
    uint        entryLen = *param_10;
    uint        data     = *pData;
    int         offs     = 0;
    uint        uVar3    = 3;
    ushort     *pEntry   = animData + 1;
    vec3s      *out      = pOut;

    do { // while(true)
        entry   = *pEntry; //r14
        entryLo = entry & 0xf;
        entryHi = entry & 0xfff0;
        if (entryLo) {
            //entryLo is a bit shift
            offs += entryLo;
            if (offs > 32) {
                byteIdx   = (offs - entryLo) >> 3; // bit idx -> byte idx
                param_10 += byteIdx; // param_10 = r12
                pData    += byteIdx;
                bitIdx    = (offs - entryLo) & 7; // bit idx within byte
                entryLen  = *param_10 << bitIdx;
                data      = *pData << bitIdx;
                offs      = bitIdx + entryLo;
            }
            lenHi      =  entryLen >> (32 - entryLo);
            uVar5      = (data     >> (32 - entryLo)) - lenHi;
            //suspicious: 0x40000 = 16384 * 16
            uVar5      = (((uVar5 * 0x40000) | (uVar5 >> 14)) >> 18) * uVar1;
            in_xer_ca  = (uVar5 < 0) && (uVar5 & 0x3fff) != 0;
            entryHi   += (lenHi + (uVar5 >> 14)) * 4;
            entryLen <<= entryLo;
            data     <<= entryLo;
        } // if(entryLo)

        out->x = entryHi;
        next = pEntry + 1;
        out[2].x = 0;
        out[4].x = 0;

        if ((entry & 0x10) != 0) {
            entry = *next;
            entryLo = entry;
            if ((entry & 0x10) != 0) {
                entryHi = entry & 0xffc0;
                entryLo &= 0xf;
                if ((entry & 0xf) != 0) {
                    offs += entryLo;
                    if (0x20 < offs) {
                        entryLen = offs - entryLo >> 3;
                        param_10 = param_10 + entryLen;
                        pData = pData + entryLen;
                        uVar5 = offs - entryLo & 7;
                        entryLen = *param_10 << uVar5;
                        data = *pData << uVar5;
                        offs = uVar5 + entryLo;
                    }
                    uVar7 = entryLen >> 0x20 - entryLo;
                    uVar5 = ((data >> 0x20 - entryLo) - uVar7) * uVar1;
                    in_xer_ca = uVar5 < 0 && (uVar5 & 0x3fff) != 0;
                    entryHi += (uVar7 + (uVar5 >> 0xe)) * 2;
                    entryLen <<= entryLo;
                    data <<= entryLo;
                }
                out[2].x = entryHi;
                next = pEntry + 2;
                entryLo = *next;
                if ((entry & 0x20) == 0) goto LAB_800075d4;
            } //if(entry & 0x10)

            entry = entryLo & 0xfff0;
            entryLo &= 0xf;
            if (entryLo != 0) {
                offs += entryLo;
                if (0x20 < offs) {
                    entryLen = offs - entryLo >> 3;
                    param_10 = param_10 + entryLen;
                    pData = pData + entryLen;
                    uVar5 = offs - entryLo & 7;
                    entryLen = *param_10 << uVar5;
                    data = *pData << uVar5;
                    offs = uVar5 + entryLo;
                }
                uVar7 = entryLen >> 0x20 - entryLo;
                uVar5 = (data >> 0x20 - entryLo) - uVar7;
                uVar5 = ((uVar5 * 0x10000 | uVar5 >> 0x10) >> 0x10) * uVar1;
                in_xer_ca = uVar5 < 0 && (uVar5 & 0x3fff) != 0;
                entry += uVar7 + (uVar5 >> 0xe);
                entryLen <<= entryLo;
                data <<= entryLo;
            }
            out[4].x = entry;
            next = next + 1;
        }

LAB_800075d4:
        bVar8 = CARRY4(uVar3,in_xer_ca - 1);
        uVar3 += in_xer_ca - 1;
        if (uVar3 == 0) {
            uVar3 = 3;
            out = &out[9].z;
        }
        out = &out->y;
        entryLo = bVar8 - 1;
        in_xer_ca = CARRY4(uVar2,entryLo);
        uVar2 += entryLo;
        pEntry = next;
        if (uVar2 == 0) {
            while (*param_6 != 0x1000) {
                psVar4 = &pOut->x + *param_6;
                *psVar4 = *psVar4 + param_6[2];
                param_6 = param_6 + 4;
            }
            return;
        }
    } while( true );
}
