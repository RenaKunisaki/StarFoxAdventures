8004b658 7C 08 02 A6  mfspr     r0,LR
8004b65c 94 21 FF AC  stwu      r1,DAT_ffffffac(r1)
8004b660 90 01 00 58  stw       r0,0x58(r1)
8004b664 BD C1 00 0C  stmw      r14,0xc(r1)
8004b668 38 A5 FF FF  subi      pOut,pOut,0x1
8004b66c 38 E0 00 00  li        r7,0x0
8004b670 39 E0 00 20  li        r15,0x20
8004b674 38 63 00 02  addi      pIn,pIn,0x2
LAB_8004b678                       XREF[1]:   8004bf6c(j)
8004b678 8A 03 00 00  lbz       r16,0x0(pIn)
8004b67c 38 E7 00 01  addi      r7,r7,0x1
8004b680 5E 10 7F FE  rlwnm     r16,r16,r15,0x1f,0x1f
8004b684 7C F1 1E 70  srawi     r17,r7,0x3
8004b688 92 01 00 08  stw       r16,0x8(r1)
8004b68c 7C 63 8A 14  add       lengths,lengths,r17
8004b690 70 E7 00 07  andi.     r7,r7,0x7
8004b694 21 E7 00 20  subfic    r15,r7,0x20
8004b698 2C 07 00 06  cmpwi     r7,0x6
8004b69c 8A 43 00 00  lbz       r18,0x0(lengths)
8004b6a0 5E 52 7E 3E  rlwnm     r18,r18,r15,0x18,0x1f
8004b6a4 22 67 00 08  subfic    r19,r7,0x8
8004b6a8 8A 83 00 01  lbz       r20,0x1(lengths)
8004b6ac 7E 93 98 30  slw       r19,r20,r19
8004b6b0 7E 52 9B 78  or        r18,r18,r19
8004b6b4 72 52 00 03  andi.     r18,r18,0x3
8004b6b8 38 E7 00 02  addi      r7,r7,0x2
8004b6bc 7C F1 1E 70  srawi     r17,r7,0x3
8004b6c0 7C 63 8A 14  add       lengths,lengths,r17
8004b6c4 70 E7 00 07  andi.     r7,r7,0x7
8004b6c8 21 E7 00 20  subfic    r15,r7,0x20
8004b6cc 2C 12 00 00  cmpwi     r18,0x0
8004b6d0 40 82 00 48  bne       LAB_8004b718
8004b6d4 2C 07 00 00  cmpwi     r7,0x0
8004b6d8 41 82 00 0C  beq       LAB_8004b6e4
8004b6dc 38 63 00 01  addi      lengths,lengths,0x1
8004b6e0 38 E0 00 00  li        r7,0x0
LAB_8004b6e4
8004b6e4 A2 43 00 00  lhz       r18,0x0(lengths)
8004b6e8 38 63 00 01  addi      lengths,lengths,0x1
8004b6ec A2 63 00 00  lhz       r19,0x0(lengths)
8004b6f0 38 63 00 03  addi      lengths,lengths,0x3
8004b6f4 56 73 40 2E  rlwinm    r19,r19,0x8,0x0,0x17
8004b6f8 7E 52 9B 78  or        r18,r18,r19
LAB_8004b6fc                       XREF[1]:   8004b710(j)
8004b6fc 8A 63 00 00  lbz       r19,0x0(lengths)
8004b700 38 63 00 01  addi      lengths,lengths,0x1
8004b704 9E 65 00 01  stbu      r19,0x1(pOut)
8004b708 7C 00 04 00  mcrxr     cr0
8004b70c 7E 52 01 D5  addme.    r18,r18
8004b710 40 82 FF EC  bne       LAB_8004b6fc
8004b714 48 00 08 50  b         LAB_8004bf64
LAB_8004b718                       XREF[1]:   8004b6d0(j)
8004b718 2C 12 00 01  cmpwi     r18,0x1
8004b71c 40 82 00 30  bne       LAB_8004b74c
8004b720 3D 00 80 31  lis       r8,DAT_ffff8031
8004b724 39 08 C8 80  subi      r8,r8,offset zlb_lengths          =
8004b728 3D 20 80 31  lis       r9,DAT_ffff8031
8004b72c 39 29 C9 A0  subi      tbl2,tbl2,offset zlb_tbl2a        =
8004b730 39 40 00 09  li        r10,0x9
8004b734 3D 60 80 31  lis       r11,DAT_ffff8031
8004b738 39 6B CD A0  subi      distances,distances,offset zlb_d  =
8004b73c 3D 80 80 31  lis       r12,DAT_ffff8031
8004b740 39 8C CD C0  subi      tbl4,tbl4,offset zlb_tbl4a        =
8004b744 39 C0 00 05  li        r14,0x5
8004b748 48 00 06 00  b         LAB_8004bd48
LAB_8004b74c                       XREF[1]:   8004b71c(j)
8004b74c 3D 00 80 36  lis       r8,DAT_ffff8036
8004b750 39 08 F7 40  subi      r8,r8,offset zlb_lengths2         =
8004b754 3D 20 80 36  lis       tbl2,DAT_ffff8036
8004b758 39 29 F8 60  subi      tbl2,tbl2,offset zlb_tbl2b        =
8004b75c 3D 60 80 37  lis       distances,DAT_ffff8037
8004b760 39 6B F8 60  subi      distances,distances,offset zlb_d  =
8004b764 3D 80 80 37  lis       tbl4,DAT_ffff8037
8004b768 39 8C F8 80  subi      tbl4,tbl4,offset zlb_tbl4b        =
8004b76c 3A 60 00 00  li        r19,0x0
8004b770 3A A0 00 08  li        r21,0x8
8004b774 7E A9 03 A6  mtspr     CTR,r21
8004b778 3E 40 80 3E  lis       r18,DAT_ffff803e
8004b77c 3A 52 CD 20  subi      tbl5,tbl5,offset zlb_tbl5         =
LAB_8004b780                       XREF[1]:   8004b788(j)
8004b780 9A 72 00 00  stb       r19,0x0(tbl5)                     =
                                              = null
8004b784 3A 52 00 01  addi      tbl5,tbl5,offset zlb_tbl5[1]      = null
8004b788 42 00 FF F8  bdnz      LAB_8004b780
8004b78c 3A A0 00 13  li        r21,0x13
8004b790 7E A9 03 A6  mtspr     CTR,r21
8004b794 3E 40 80 37  lis       tbl5,DAT_ffff8037
8004b798 3A 52 78 80  addi      tbl5,tbl5,offset zlb_tbl6         =
LAB_8004b79c                       XREF[1]:   8004b7a4(j)
8004b79c 9A 72 00 00  stb       r19,0x0(tbl5)                     =
                                              = null
8004b7a0 3A 52 00 01  addi      tbl5,tbl5,offset zlb_tbl6[1]      = null
8004b7a4 42 00 FF F8  bdnz      LAB_8004b79c
8004b7a8 3A A0 00 10  li        r21,0x10
8004b7ac 7E A9 03 A6  mtspr     CTR,r21
8004b7b0 3E 40 80 37  lis       tbl5,DAT_ffff8037
8004b7b4 3A 52 78 94  addi      tbl6,tbl6,offset zlb_tbl7         =
LAB_8004b7b8                       XREF[1]:   8004b7c0(j)
8004b7b8 B2 72 00 00  sth       r19,0x0(tbl6)                     =
                                              = null
8004b7bc 3A 52 00 02  addi      tbl6,tbl6,offset zlb_tbl7[1]      = null
8004b7c0 42 00 FF F8  bdnz      LAB_8004b7b8
8004b7c4 3A A0 01 20  li        r21,0x120
8004b7c8 7E A9 03 A6  mtspr     CTR,r21
8004b7cc 7D 12 43 78  or        tbl6,r8,r8
LAB_8004b7d0                       XREF[1]:   8004b7d8(j)
8004b7d0 9A 72 00 00  stb       r19,0x0(tbl6)                     =
                                              = null
8004b7d4 3A 52 00 01  addi      tbl6,tbl6,offset zlb_lengths2[1]  = null
8004b7d8 42 00 FF F8  bdnz      LAB_8004b7d0
8004b7dc 3A A0 00 10  li        r21,0x10
8004b7e0 7E A9 03 A6  mtspr     CTR,r21
8004b7e4 3E 40 80 37  lis       tbl6,DAT_ffff8037
8004b7e8 3A 52 78 B4  addi      tbl6,tbl6,offset zlb_tbl8         =
LAB_8004b7ec                       XREF[1]:   8004b7f4(j)
8004b7ec B2 72 00 00  sth       r19,0x0(tbl6)                     =
                                              = null
8004b7f0 3A 52 00 02  addi      tbl6,tbl6,offset zlb_tbl8[1]      = null
8004b7f4 42 00 FF F8  bdnz      LAB_8004b7ec
8004b7f8 3A A0 00 20  li        r21,0x20
8004b7fc 7E A9 03 A6  mtspr     CTR,r21
8004b800 7D 72 5B 78  or        tbl6,distances,distances
LAB_8004b804                       XREF[1]:   8004b80c(j)
8004b804 9A 72 00 00  stb       r19,0x0(tbl6)                     =
                                              = null
8004b808 3A 52 00 01  addi      tbl6,tbl6,offset zlb_distances2[  = null
8004b80c 42 00 FF F8  bdnz      LAB_8004b804
8004b810 8A 43 00 01  lbz       tbl6,0x1(lengths)
8004b814 22 27 00 08  subfic    r17,r7,0x8
8004b818 7E 52 88 30  slw       tbl6,tbl6,r17
8004b81c 8A 23 00 00  lbz       r17,0x0(lengths)
8004b820 5E 31 7E 3E  rlwnm     r17,r17,r15,0x18,0x1f
8004b824 7E 31 93 78  or        r17,r17,tbl6
8004b828 72 31 00 1F  andi.     r17,r17,0x1f
8004b82c 38 E7 00 05  addi      r7,r7,0x5
8004b830 3A F1 01 01  addi      r23,r17,0x101
8004b834 7C F1 1E 70  srawi     r17,r7,0x3
8004b838 7C 63 8A 14  add       lengths,lengths,r17
8004b83c 70 E7 00 07  andi.     r7,r7,0x7
8004b840 21 E7 00 20  subfic    r15,r7,0x20
8004b844 8A 43 00 01  lbz       tbl6,0x1(lengths)
8004b848 22 27 00 08  subfic    r17,r7,0x8
8004b84c 7E 52 88 30  slw       tbl6,tbl6,r17
8004b850 8A 23 00 00  lbz       r17,0x0(lengths)
8004b854 5E 31 7E 3E  rlwnm     r17,r17,r15,0x18,0x1f
8004b858 7E 31 93 78  or        r17,r17,tbl6
8004b85c 72 31 00 1F  andi.     r17,r17,0x1f
8004b860 38 E7 00 05  addi      r7,r7,0x5
8004b864 3B 11 00 01  addi      r24,r17,0x1
8004b868 7C F1 1E 70  srawi     r17,r7,0x3
8004b86c 7C 63 8A 14  add       lengths,lengths,r17
8004b870 70 E7 00 07  andi.     r7,r7,0x7
8004b874 21 E7 00 20  subfic    r15,r7,0x20
8004b878 8A 43 00 01  lbz       tbl6,0x1(lengths)
8004b87c 22 27 00 08  subfic    r17,r7,0x8
8004b880 7E 52 88 30  slw       tbl6,tbl6,r17
8004b884 8A 23 00 00  lbz       r17,0x0(lengths)
8004b888 5E 31 7E 3E  rlwnm     r17,r17,r15,0x18,0x1f
8004b88c 7E 31 93 78  or        r17,r17,tbl6
8004b890 72 31 00 0F  andi.     r17,r17,0xf
8004b894 38 E7 00 04  addi      r7,r7,0x4
8004b898 3B 31 00 04  addi      r25,r17,0x4
8004b89c 7C F1 1E 70  srawi     r17,r7,0x3
8004b8a0 7C 63 8A 14  add       lengths,lengths,r17
8004b8a4 70 E7 00 07  andi.     r7,r7,0x7
8004b8a8 21 E7 00 20  subfic    r15,r7,0x20
8004b8ac 3F A0 80 2C  lis       r29,DAT_ffff802c
8004b8b0 3F C0 80 37  lis       r30,DAT_ffff8037
8004b8b4 3F E0 80 3E  lis       r31,DAT_ffff803e
8004b8b8 3B BD 1C 50  addi      r29,r29,offset zlb_order          =
8004b8bc 3B DE 78 80  addi      r30,r30,offset zlb_tbl6           =
8004b8c0 3B FF CD 20  subi      r31,r31,offset zlb_tbl5           =
8004b8c4 3A A0 00 00  li        r21,0x0
8004b8c8 48 00 00 4C  b         LAB_8004b914
LAB_8004b8cc                       XREF[1]:   8004b918(j)
8004b8cc 8A 43 00 01  lbz       tbl6,0x1(lengths)
8004b8d0 22 27 00 08  subfic    r17,r7,0x8
8004b8d4 7E 52 88 30  slw       tbl6,tbl6,r17
8004b8d8 8A 23 00 00  lbz       r17,0x0(lengths)
8004b8dc 5E 31 7E 3E  rlwnm     r17,r17,r15,0x18,0x1f
8004b8e0 7E 31 93 78  or        r17,r17,tbl6
8004b8e4 72 31 00 07  andi.     r17,r17,0x7
8004b8e8 7E 5D A8 AE  lbzx      tbl6,r29,r21                      =
8004b8ec 7E 3E 91 AE  stbx      r17,r30,tbl6                      = null
8004b8f0 7E 5F 88 AE  lbzx      tbl6,r31,r17                      =
8004b8f4 3A 52 00 01  addi      tbl6,tbl6,0x1
8004b8f8 38 E7 00 03  addi      r7,r7,0x3
8004b8fc 7E 5F 89 AE  stbx      tbl6,r31,r17                      =
8004b900 7C F1 1E 70  srawi     r17,r7,0x3
8004b904 7C 63 8A 14  add       lengths,lengths,r17
8004b908 70 E7 00 07  andi.     r7,r7,0x7
8004b90c 21 E7 00 20  subfic    r15,r7,0x20
8004b910 3A B5 00 01  addi      r21,r21,0x1
LAB_8004b914                       XREF[1]:   8004b8c8(j)
8004b914 7C 15 C8 00  cmpw      r21,r25
8004b918 40 82 FF B4  bne       LAB_8004b8cc
8004b91c 38 80 00 07  li        compLen,0x7
8004b920 3E 00 80 3E  lis       r16,DAT_ffff803e
8004b924 3A 10 CD 20  subi      r16,r16,offset zlb_tbl5           =
LAB_8004b928                       XREF[1]:   8004b938(j)
8004b928 7E 44 80 AE  lbzx      tbl6,compLen,r16                  = null
8004b92c 2C 12 00 00  cmpwi     tbl6,0x0
8004b930 40 82 00 0C  bne       LAB_8004b93c
8004b934 38 84 FF FF  subi      compLen,compLen,0x1
8004b938 4B FF FF F0  b         LAB_8004b928
LAB_8004b93c                       XREF[1]:   8004b930(j)
8004b93c 3B 40 00 01  li        r26,0x1
8004b940 3A C0 00 00  li        r22,0x0
8004b944 3F E0 80 3E  lis       r31,DAT_ffff803e
8004b948 3B FF CD 18  subi      r31,r31,offset zlb_tbl10          =
8004b94c 48 00 00 24  b         LAB_8004b970
LAB_8004b950                       XREF[1]:   8004b974(j)
8004b950 7E 5A 80 AE  lbzx      tbl6,r26,r16                      = null
8004b954 2C 12 00 00  cmpwi     tbl6,0x0
8004b958 41 82 00 14  beq       LAB_8004b96c
8004b95c 7E DF D1 AE  stbx      r22,r31,r26                       = null
8004b960 7E 3A 20 50  subf      r17,r26,compLen
8004b964 7E 51 88 30  slw       r17,tbl6,r17
8004b968 7E D6 8A 14  add       r22,r22,r17
LAB_8004b96c                       XREF[1]:   8004b958(j)
8004b96c 3B 5A 00 01  addi      r26,r26,0x1
LAB_8004b970                       XREF[1]:   8004b94c(j)
8004b970 7C 1A 20 00  cmpw      r26,compLen
8004b974 40 81 FF DC  ble       LAB_8004b950
8004b978 3E 00 80 37  lis       r16,DAT_ffff8037
8004b97c 3A 10 78 80  addi      r16,r16,offset zlb_tbl6           =
8004b980 3B 80 00 00  li        r28,0x0
8004b984 3F 60 80 37  lis       r27,DAT_ffff8037
8004b988 3B 7B 78 D4  addi      r27,r27,offset zlb_tbl11          =
8004b98c 3B 7B FF FF  subi      r27,r27,offset zlb_tbl8[15]+1     = null
8004b990 48 00 00 4C  b         LAB_8004b9dc
LAB_8004b994                       XREF[1]:   8004b9e0(j)
8004b994 3F A0 80 3E  lis       r29,DAT_ffff803e
8004b998 3B BD CD 18  subi      r29,r29,offset zlb_tbl10          =
8004b99c 7E 30 E0 AE  lbzx      r17,r16,r28                       =
8004b9a0 2C 11 00 00  cmpwi     r17,0x0
8004b9a4 41 82 00 34  beq       LAB_8004b9d8
8004b9a8 3B E0 00 00  li        r31,0x0
8004b9ac 48 00 00 18  b         LAB_8004b9c4
LAB_8004b9b0                       XREF[1]:   8004b9d4(j)
8004b9b0 7F D1 E8 AE  lbzx      r30,r17,r29                       =
8004b9b4 3B DE 00 01  addi      r30,r30,0x1
8004b9b8 7F D1 E9 AE  stbx      r30,r17,r29                       =
8004b9bc 7F 9B F1 AE  stbx      r28,r27,r30                       =
8004b9c0 3B FF 00 01  addi      r31,r31,0x1
LAB_8004b9c4                       XREF[1]:   8004b9ac(j)
8004b9c4 7E 51 20 50  subf      tbl6,r17,compLen
8004b9c8 3A C0 00 01  li        r22,0x1
8004b9cc 7E D2 90 30  slw       tbl6,r22,tbl6
8004b9d0 7C 1F 90 00  cmpw      r31,tbl6
8004b9d4 41 80 FF DC  blt       LAB_8004b9b0
LAB_8004b9d8                       XREF[1]:   8004b9a4(j)
8004b9d8 3B 9C 00 01  addi      r28,r28,0x1
LAB_8004b9dc                       XREF[1]:   8004b990(j)
8004b9dc 2C 1C 00 13  cmpwi     r28,0x13
8004b9e0 41 80 FF B4  blt       LAB_8004b994
8004b9e4 7D 13 43 78  or        r19,r8,r8
8004b9e8 3E 80 80 37  lis       r20,DAT_ffff8037
8004b9ec 3A 94 78 94  addi      r20,r20,offset zlb_tbl7           =
8004b9f0 3A A0 00 00  li        r21,0x0
LAB_8004b9f4                       XREF[2]:   8004bb90(j), 8004bb98(j)
8004b9f4 22 04 00 08  subfic    r16,compLen,0x8
8004b9f8 7C 07 80 00  cmpw      r7,r16
8004b9fc 3A 40 00 00  li        tbl6,0x0
8004ba00 40 81 00 10  ble       LAB_8004ba10
8004ba04 8A 43 00 01  lbz       tbl6,0x1(lengths)
8004ba08 22 27 00 08  subfic    r17,r7,0x8
8004ba0c 7E 52 88 30  slw       tbl6,tbl6,r17
LAB_8004ba10                       XREF[1]:   8004ba00(j)
8004ba10 8A 23 00 00  lbz       r17,0x0(lengths)
8004ba14 5E 31 7E 3E  rlwnm     r17,r17,r15,0x18,0x1f
8004ba18 7E 31 93 78  or        r17,r17,tbl6
8004ba1c 3A 40 00 01  li        tbl6,0x1
8004ba20 7E 52 20 30  slw       tbl6,tbl6,compLen
8004ba24 3A 52 FF FF  subi      tbl6,tbl6,0x1
8004ba28 7E 3B 90 38  and       r27,r17,tbl6
8004ba2c 3F E0 80 31  lis       r31,DAT_ffff8031
8004ba30 3B FF CD E0  subi      r31,r31,offset zlb_tbl12          =
8004ba34 3A 24 00 18  addi      r17,compLen,0x18
8004ba38 7E 1B F8 AE  lbzx      r16,r27,r31                       =
8004ba3c 5E 1B 8E 3E  rlwnm     r27,r16,r17,0x18,0x1f
8004ba40 3E 00 80 37  lis       r16,DAT_ffff8037
8004ba44 3A 10 78 D4  addi      r16,r16,offset zlb_tbl11          =
8004ba48 7E 10 DA 14  add       r16,r16,r27
8004ba4c 8B 90 00 00  lbz       pbVar16,0x0(r16)                  =
8004ba50 3E 00 80 37  lis       r16,DAT_ffff8037
8004ba54 3A 10 78 80  addi      r16,r16,offset zlb_tbl6           =
8004ba58 7E 10 E2 14  add       r16,r16,pbVar16
8004ba5c 8A 10 00 00  lbz       r16,0x0(r16)                      =
8004ba60 7C E7 82 14  add       r7,r7,r16
8004ba64 7C F1 1E 70  srawi     r17,r7,0x3
8004ba68 7C 63 8A 14  add       lengths,lengths,r17
8004ba6c 70 E7 00 07  andi.     r7,r7,0x7
8004ba70 2C 1C 00 10  cmpwi     pbVar16,0x10
8004ba74 21 E7 00 20  subfic    r15,r7,0x20
8004ba78 40 82 00 3C  bne       LAB_8004bab4
8004ba7c 8A 43 00 01  lbz       tbl6,0x1(lengths)
8004ba80 22 27 00 08  subfic    r17,r7,0x8
8004ba84 7E 52 88 30  slw       tbl6,tbl6,r17
8004ba88 8A 23 00 00  lbz       r17,0x0(lengths)
8004ba8c 5E 31 7E 3E  rlwnm     r17,r17,r15,0x18,0x1f
8004ba90 7E 31 93 78  or        r17,r17,tbl6
8004ba94 72 31 00 03  andi.     r17,r17,0x3
8004ba98 38 E7 00 02  addi      r7,r7,0x2
8004ba9c 3B D1 00 03  addi      r30,r17,0x3
8004baa0 7C F1 1E 70  srawi     r17,r7,0x3
8004baa4 7C 63 8A 14  add       lengths,lengths,r17
8004baa8 70 E7 00 07  andi.     r7,r7,0x7
8004baac 21 E7 00 20  subfic    r15,r7,0x20
8004bab0 48 00 00 94  b         LAB_8004bb44
LAB_8004bab4                       XREF[1]:   8004ba78(j)
8004bab4 2C 1C 00 11  cmpwi     pbVar16,0x11
8004bab8 40 82 00 40  bne       LAB_8004baf8
8004babc 3B A0 00 00  li        r29,0x0
8004bac0 8A 43 00 01  lbz       tbl6,0x1(lengths)
8004bac4 22 27 00 08  subfic    r17,r7,0x8
8004bac8 7E 52 88 30  slw       tbl6,tbl6,r17
8004bacc 8A 23 00 00  lbz       r17,0x0(lengths)
8004bad0 5E 31 7E 3E  rlwnm     r17,r17,r15,0x18,0x1f
8004bad4 7E 31 93 78  or        r17,r17,tbl6
8004bad8 72 31 00 07  andi.     r17,r17,0x7
8004badc 38 E7 00 03  addi      r7,r7,0x3
8004bae0 3B D1 00 03  addi      r30,r17,0x3
8004bae4 7C F1 1E 70  srawi     r17,r7,0x3
8004bae8 7C 63 8A 14  add       lengths,lengths,r17
8004baec 70 E7 00 07  andi.     r7,r7,0x7
8004baf0 21 E7 00 20  subfic    r15,r7,0x20
8004baf4 48 00 00 50  b         LAB_8004bb44
LAB_8004baf8                       XREF[1]:   8004bab8(j)
8004baf8 2C 1C 00 12  cmpwi     pbVar16,0x12
8004bafc 40 82 00 40  bne       LAB_8004bb3c
8004bb00 3B A0 00 00  li        r29,0x0
8004bb04 8A 43 00 01  lbz       tbl6,0x1(lengths)
8004bb08 22 27 00 08  subfic    r17,r7,0x8
8004bb0c 7E 52 88 30  slw       tbl6,tbl6,r17
8004bb10 8A 23 00 00  lbz       r17,0x0(lengths)
8004bb14 5E 31 7E 3E  rlwnm     r17,r17,r15,0x18,0x1f
8004bb18 7E 31 93 78  or        r17,r17,tbl6
8004bb1c 72 31 00 7F  andi.     r17,r17,0x7f
8004bb20 38 E7 00 07  addi      r7,r7,0x7
8004bb24 3B D1 00 0B  addi      r30,r17,0xb
8004bb28 7C F1 1E 70  srawi     r17,r7,0x3
8004bb2c 7C 63 8A 14  add       lengths,lengths,r17
8004bb30 70 E7 00 07  andi.     r7,r7,0x7
8004bb34 21 E7 00 20  subfic    r15,r7,0x20
8004bb38 48 00 00 0C  b         LAB_8004bb44
LAB_8004bb3c                       XREF[1]:   8004bafc(j)
8004bb3c 7F 9D E3 78  or        r29,pbVar16,pbVar16
8004bb40 3B C0 00 01  li        r30,0x1
LAB_8004bb44                       XREF[4]:   8004bab0(j), 8004baf4(j), 8004bb38(j), 8004bb88(j)
8004bb44 7F B3 A9 AE  stbx      r29,r19,r21                       =
8004bb48 3A B5 00 01  addi      r21,r21,0x1
8004bb4c 7E 14 EA 14  add       r16,r20,r29
8004bb50 7E 10 EA 14  add       r16,r16,r29
8004bb54 A2 30 00 00  lhz       r17,0x0(r16)                      = 8B2Dh
                                              =
                                              = EF2Fh
8004bb58 3A 31 00 01  addi      r17,r17,0x1
8004bb5c B2 30 00 00  sth       r17,0x0(r16)                      = 8B2Dh
                                              =
                                              = EF2Fh
8004bb60 7C 13 40 00  cmpw      r19,r8
8004bb64 40 82 00 1C  bne       LAB_8004bb80
8004bb68 7C 15 B8 00  cmpw      r21,r23
8004bb6c 40 82 00 14  bne       LAB_8004bb80
8004bb70 7D 73 5B 78  or        r19,distances,distances
8004bb74 3E 80 80 37  lis       r20,DAT_ffff8037
8004bb78 3A 94 78 B4  addi      r20,r20,offset zlb_tbl8           =
8004bb7c 3A A0 00 00  li        r21,0x0
LAB_8004bb80                       XREF[2]:   8004bb64(j), 8004bb6c(j)
8004bb80 7C 00 04 00  mcrxr     cr0
8004bb84 7F DE 01 D5  addme.    r30,r30
8004bb88 40 82 FF BC  bne       LAB_8004bb44
8004bb8c 7C 13 40 00  cmpw      r19,r8
8004bb90 41 82 FE 64  beq       LAB_8004b9f4
8004bb94 7C 15 C0 00  cmpw      r21,r24
8004bb98 41 80 FE 5C  blt       LAB_8004b9f4
8004bb9c 39 40 00 0F  li        r10,0xf
8004bba0 3E 00 80 37  lis       r16,DAT_ffff8037
8004bba4 3A 10 78 94  addi      r16,r16,offset zlb_tbl7           =
8004bba8 7E 50 52 14  add       tbl6,r16,r10
8004bbac 7E 52 52 14  add       tbl6,tbl6,r10
LAB_8004bbb0                       XREF[1]:   8004bbc4(j)
8004bbb0 A2 32 00 00  lhz       r17,0x0(tbl6)                     = null
8004bbb4 2C 11 00 00  cmpwi     r17,0x0
8004bbb8 40 82 00 10  bne       LAB_8004bbc8
8004bbbc 3A 52 FF FE  subi      tbl6,tbl6,offset zlb_tbl7[14]     = null
8004bbc0 39 4A FF FF  subi      r10,r10,0x1
8004bbc4 4B FF FF EC  b         LAB_8004bbb0
LAB_8004bbc8                       XREF[1]:   8004bbb8(j)
8004bbc8 3F C0 80 37  lis       r30,DAT_ffff8037
8004bbcc 3B DE 79 54  addi      r30,r30,offset zlb_tbl13          =
8004bbd0 3B 40 00 01  li        r26,0x1
8004bbd4 3A C0 00 00  li        r22,0x0
8004bbd8 48 00 00 34  b         LAB_8004bc0c
LAB_8004bbdc                       XREF[1]:   8004bc10(j)
8004bbdc 7E 30 D2 14  add       r17,r16,r26
8004bbe0 7E 31 D2 14  add       r17,r17,r26
8004bbe4 A2 31 00 00  lhz       r17,0x0(r17)                      = null
8004bbe8 2C 11 00 00  cmpwi     r17,0x0
8004bbec 41 82 00 1C  beq       LAB_8004bc08
8004bbf0 7F 9E D2 14  add       pbVar16,r30,r26
8004bbf4 7F 9C D2 14  add       pbVar16,pbVar16,r26
8004bbf8 B2 DC 00 00  sth       r22,0x0(pbVar16)                  = null
8004bbfc 7F 9A 50 50  subf      pbVar16,r26,r10
8004bc00 7E 31 E0 30  slw       r17,r17,pbVar16
8004bc04 7E D6 8A 14  add       r22,r22,r17
LAB_8004bc08                       XREF[1]:   8004bbec(j)
8004bc08 3B 5A 00 01  addi      r26,r26,0x1
LAB_8004bc0c                       XREF[1]:   8004bbd8(j)
8004bc0c 7C 1A 50 00  cmpw      r26,r10
8004bc10 40 81 FF CC  ble       LAB_8004bbdc
8004bc14 3B 80 00 00  li        pbVar16,0x0
8004bc18 48 00 00 58  b         LAB_8004bc70
LAB_8004bc1c                       XREF[1]:   8004bc74(j)
8004bc1c 7E 28 E0 AE  lbzx      r17,r8,pbVar16                    =
8004bc20 2C 11 00 00  cmpwi     r17,0x0
8004bc24 41 82 00 48  beq       LAB_8004bc6c
8004bc28 3B E0 00 00  li        r31,0x0
8004bc2c 48 00 00 2C  b         LAB_8004bc58
LAB_8004bc30                       XREF[1]:   8004bc68(j)
8004bc30 7E 5E 8A 14  add       tbl6,r30,r17
8004bc34 7F B2 8A 14  add       r29,tbl6,r17
8004bc38 A2 5D 00 00  lhz       tbl6,0x0(r29)                     = null
8004bc3c 3A 52 00 01  addi      tbl6,tbl6,0x1
8004bc40 B2 5D 00 00  sth       tbl6,0x0(r29)                     = null
8004bc44 3A 52 FF FF  subi      tbl6,tbl6,0x1
8004bc48 7F A9 92 14  add       r29,tbl2,tbl6
8004bc4c 7F BD 92 14  add       r29,r29,tbl6
8004bc50 B3 9D 00 00  sth       pbVar16,0x0(r29)                  =
8004bc54 3B FF 00 01  addi      r31,r31,0x1
LAB_8004bc58                       XREF[1]:   8004bc2c(j)
8004bc58 7F B1 50 50  subf      r29,r17,r10
8004bc5c 3A 40 00 01  li        tbl6,0x1
8004bc60 7E 52 E8 30  slw       tbl6,tbl6,r29
8004bc64 7C 1F 90 00  cmpw      r31,tbl6
8004bc68 41 80 FF C8  blt       LAB_8004bc30
LAB_8004bc6c                       XREF[1]:   8004bc24(j)
8004bc6c 3B 9C 00 01  addi      pbVar16,pbVar16,0x1
LAB_8004bc70                       XREF[1]:   8004bc18(j)
8004bc70 7C 1C B8 00  cmpw      pbVar16,r23
8004bc74 41 80 FF A8  blt       LAB_8004bc1c
8004bc78 39 C0 00 0F  li        r14,0xf
8004bc7c 3E 00 80 37  lis       r16,DAT_ffff8037
8004bc80 3A 10 78 B4  addi      r16,r16,offset zlb_tbl8           =
8004bc84 3F 60 80 37  lis       r27,DAT_ffff8037
8004bc88 3B 7B 79 74  addi      r27,r27,offset zlb_tbl14          =
LAB_8004bc8c                       XREF[1]:   8004bca4(j)
8004bc8c 7E 30 72 14  add       r17,r16,r14
8004bc90 7E 31 72 14  add       r17,r17,r14
8004bc94 A2 51 00 00  lhz       tbl6,0x0(r17)                     = null
8004bc98 2C 12 00 00  cmpwi     tbl6,0x0
8004bc9c 40 82 00 0C  bne       LAB_8004bca8
8004bca0 39 CE FF FF  subi      r14,r14,0x1
8004bca4 4B FF FF E8  b         LAB_8004bc8c
LAB_8004bca8                       XREF[1]:   8004bc9c(j)
8004bca8 3B 40 00 01  li        r26,0x1
8004bcac 3A C0 00 00  li        r22,0x0
8004bcb0 48 00 00 34  b         LAB_8004bce4
LAB_8004bcb4                       XREF[1]:   8004bce8(j)
8004bcb4 7E 30 D2 14  add       r17,r16,r26
8004bcb8 7E 31 D2 14  add       r17,r17,r26
8004bcbc A2 51 00 00  lhz       tbl6,0x0(r17)                     = null
8004bcc0 2C 12 00 00  cmpwi     tbl6,0x0
8004bcc4 41 82 00 1C  beq       LAB_8004bce0
8004bcc8 7E 3B D2 14  add       r17,r27,r26
8004bccc 7E 31 D2 14  add       r17,r17,r26
8004bcd0 B2 D1 00 00  sth       r22,0x0(r17)                      = null
8004bcd4 7F 9A 70 50  subf      pbVar16,r26,r14
8004bcd8 7E 52 E0 30  slw       tbl6,tbl6,pbVar16
8004bcdc 7E D6 92 14  add       r22,r22,tbl6
LAB_8004bce0                       XREF[1]:   8004bcc4(j)
8004bce0 3B 5A 00 01  addi      r26,r26,0x1
LAB_8004bce4                       XREF[1]:   8004bcb0(j)
8004bce4 7C 1A 70 00  cmpw      r26,r14
8004bce8 40 81 FF CC  ble       LAB_8004bcb4
8004bcec 3B 40 00 00  li        r26,0x0
8004bcf0 48 00 00 50  b         LAB_8004bd40
LAB_8004bcf4                       XREF[1]:   8004bd44(j)
8004bcf4 7E 2B D0 AE  lbzx      r17,distances,r26                 =
8004bcf8 2C 11 00 00  cmpwi     r17,0x0
8004bcfc 41 82 00 40  beq       LAB_8004bd3c
8004bd00 3B E0 00 00  li        r31,0x0
8004bd04 48 00 00 24  b         LAB_8004bd28
LAB_8004bd08                       XREF[1]:   8004bd38(j)
8004bd08 7E 5B 8A 14  add       tbl6,r27,r17
8004bd0c 7E 52 8A 14  add       tbl6,tbl6,r17
8004bd10 A3 92 00 00  lhz       pbVar16,0x0(tbl6)                 =
8004bd14 3B 9C 00 01  addi      pbVar16,pbVar16,0x1
8004bd18 B3 92 00 00  sth       pbVar16,0x0(tbl6)                 =
8004bd1c 3B 9C FF FF  subi      pbVar16,pbVar16,0x1
8004bd20 7F 4C E1 AE  stbx      r26,tbl4,pbVar16                  =
8004bd24 3B FF 00 01  addi      r31,r31,0x1
LAB_8004bd28                       XREF[1]:   8004bd04(j)
8004bd28 7E 51 70 50  subf      tbl6,r17,r14
8004bd2c 3B A0 00 01  li        r29,0x1
8004bd30 7F B2 90 30  slw       tbl6,r29,tbl6
8004bd34 7C 1F 90 00  cmpw      r31,tbl6
8004bd38 41 80 FF D0  blt       LAB_8004bd08
LAB_8004bd3c                       XREF[1]:   8004bcfc(j)
8004bd3c 3B 5A 00 01  addi      r26,r26,0x1
LAB_8004bd40                       XREF[1]:   8004bcf0(j)
8004bd40 7C 1A C0 00  cmpw      r26,r24
8004bd44 41 80 FF B0  blt       LAB_8004bcf4
LAB_8004bd48                       XREF[2]:   8004b748(j), 8004bf60(j)
8004bd48 3F E0 80 31  lis       r31,DAT_ffff8031
8004bd4c 3B FF CD E0  subi      r31,r31,offset zlb_tbl12          =
8004bd50 8A 63 00 02  lbz       r19,0x2(lengths)
8004bd54 22 27 00 10  subfic    r17,r7,0x10
8004bd58 7E 73 88 30  slw       r19,r19,r17
8004bd5c 8A 43 00 01  lbz       tbl6,0x1(lengths)
8004bd60 22 27 00 08  subfic    r17,r7,0x8
8004bd64 7E 52 88 30  slw       tbl6,tbl6,r17
8004bd68 8A 23 00 00  lbz       r17,0x0(lengths)
8004bd6c 5E 31 7E 3E  rlwnm     r17,r17,r15,0x18,0x1f
8004bd70 7E 31 93 78  or        r17,r17,tbl6
8004bd74 7E 31 9B 78  or        r17,r17,r19
8004bd78 3A 40 00 01  li        tbl6,0x1
8004bd7c 7E 52 50 30  slw       tbl6,tbl6,r10
8004bd80 3A 52 FF FF  subi      tbl6,tbl6,0x1
8004bd84 7E 38 90 38  and       r24,r17,tbl6
8004bd88 3A 2A FF F8  subi      r17,r10,0x8
8004bd8c 73 10 00 FF  andi.     r16,r24,0xff
8004bd90 7E 10 F8 AE  lbzx      r16,r16,r31                       =
8004bd94 57 18 C2 3E  rlwinm    r24,r24,0x18,0x8,0x1f
8004bd98 5E 10 8C 3E  rlwnm     r16,r16,r17,0x10,0x1f
8004bd9c 3A 2A 00 10  addi      r17,r10,0x10
8004bda0 7F 18 F8 AE  lbzx      r24,r24,r31                       =
8004bda4 5F 11 8E 3E  rlwnm     r17,r24,r17,0x18,0x1f
8004bda8 7E 18 8B 78  or        r24,r16,r17
8004bdac 7E 09 C2 14  add       r16,tbl2,r24
8004bdb0 7E 10 C2 14  add       r16,r16,r24
8004bdb4 A3 30 00 00  lhz       r25,0x0(r16)
8004bdb8 7E 08 CA 14  add       r16,r8,r25
8004bdbc 8B 90 00 00  lbz       pbVar16,0x0(r16)                  =
8004bdc0 7C E7 E2 14  add       r7,r7,pbVar16
8004bdc4 7C F1 1E 70  srawi     r17,r7,0x3
8004bdc8 7C 63 8A 14  add       lengths,lengths,r17
8004bdcc 70 E7 00 07  andi.     r7,r7,0x7
8004bdd0 21 E7 00 20  subfic    r15,r7,0x20
8004bdd4 2C 19 01 00  cmpwi     r25,0x100
8004bdd8 40 80 00 0C  bge       LAB_8004bde4
8004bddc 9F 25 00 01  stbu      r25,0x1(pOut)
8004bde0 48 00 01 7C  b         LAB_8004bf5c
LAB_8004bde4                       XREF[1]:   8004bdd8(j)
8004bde4 41 82 01 80  beq       LAB_8004bf64
8004bde8 3A 19 FE FF  subi      r16,r25,0x101
8004bdec 56 10 10 3A  rlwinm    r16,r16,0x2,0x0,0x1d
8004bdf0 3E 20 80 2C  lis       r17,DAT_ffff802c
8004bdf4 3A 31 1C 64  addi      r17,r17,offset zlb_lbase          =
8004bdf8 7F D1 82 2E  lhzx      r30,r17,r16                       =
8004bdfc 3A 31 00 02  addi      r17,r17,offset zlb_lbase[1]       = null
8004be00 7F B1 82 2E  lhzx      r29,r17,r16                       = null
8004be04 2C 1D 00 00  cmpwi     r29,0x0
8004be08 41 82 00 44  beq       LAB_8004be4c
8004be0c 8A 43 00 01  lbz       tbl6,0x1(lengths)
8004be10 22 27 00 08  subfic    r17,r7,0x8
8004be14 7E 52 88 30  slw       tbl6,tbl6,r17
8004be18 8A 23 00 00  lbz       r17,0x0(lengths)
8004be1c 5E 31 7E 3E  rlwnm     r17,r17,r15,0x18,0x1f
8004be20 7E 31 93 78  or        r17,r17,tbl6
8004be24 3A 40 00 01  li        tbl6,0x1
8004be28 7E 52 E8 30  slw       tbl6,tbl6,r29
8004be2c 3A 52 FF FF  subi      tbl6,tbl6,0x1
8004be30 7E 30 90 38  and       r16,r17,tbl6
8004be34 7F DE 82 14  add       r30,r30,r16
8004be38 7C E7 EA 14  add       r7,r7,r29
8004be3c 7C F1 1E 70  srawi     r17,r7,0x3
8004be40 7C 63 8A 14  add       lengths,lengths,r17
8004be44 70 E7 00 07  andi.     r7,r7,0x7
8004be48 21 E7 00 20  subfic    r15,r7,0x20
LAB_8004be4c                       XREF[1]:   8004be08(j)
8004be4c 3F E0 80 31  lis       r31,DAT_ffff8031
8004be50 3B FF CD E0  subi      r31,r31,offset zlb_tbl12          =
8004be54 8A 63 00 02  lbz       r19,0x2(lengths)
8004be58 22 27 00 10  subfic    r17,r7,0x10
8004be5c 7E 73 88 30  slw       r19,r19,r17
8004be60 8A 43 00 01  lbz       tbl6,0x1(lengths)
8004be64 22 27 00 08  subfic    r17,r7,0x8
8004be68 7E 52 88 30  slw       tbl6,tbl6,r17
8004be6c 8A 23 00 00  lbz       r17,0x0(lengths)
8004be70 5E 31 7E 3E  rlwnm     r17,r17,r15,0x18,0x1f
8004be74 7E 31 93 78  or        r17,r17,tbl6
8004be78 7E 31 9B 78  or        r17,r17,r19
8004be7c 3A 40 00 01  li        tbl6,0x1
8004be80 7E 52 70 30  slw       tbl6,tbl6,r14
8004be84 3A 52 FF FF  subi      tbl6,tbl6,0x1
8004be88 7E 3A 90 38  and       r26,r17,tbl6
8004be8c 3A 2E FF F8  subi      r17,r14,offset DAT_fffffffd
8004be90 73 50 00 FF  andi.     r16,r26,0xff
8004be94 7E 10 F8 AE  lbzx      r16,r16,r31                       =
8004be98 57 5A C2 3E  rlwinm    r26,r26,0x18,0x8,0x1f
8004be9c 5E 10 8C 3E  rlwnm     r16,r16,r17,0x10,0x1f
8004bea0 3A 2E 00 10  addi      r17,r14,0x10
8004bea4 7F 5A F8 AE  lbzx      r26,r26,r31                       =
8004bea8 5F 51 8E 3E  rlwnm     r17,r26,r17,0x18,0x1f
8004beac 7E 1A 8B 78  or        r26,r16,r17
8004beb0 7E 0C D2 14  add       r16,tbl4,r26
8004beb4 8B 70 00 00  lbz       r27,0x0(r16)                      =
8004beb8 7E 0B DA 14  add       r16,distances,r27
8004bebc 8B 90 00 00  lbz       pbVar16,0x0(r16)                  =
8004bec0 7C E7 E2 14  add       r7,r7,pbVar16
8004bec4 7C F1 1E 70  srawi     r17,r7,0x3
8004bec8 7C 63 8A 14  add       lengths,lengths,r17
8004becc 70 E7 00 07  andi.     r7,r7,0x7
8004bed0 21 E7 00 20  subfic    r15,r7,0x20
8004bed4 3E 00 80 2C  lis       r16,DAT_ffff802c
8004bed8 3A 10 1C D8  addi      r16,r16,offset zlb_dbase          =
8004bedc 57 7B 10 3A  rlwinm    r27,r27,0x2,0x0,0x1d
8004bee0 7E 10 DA 14  add       r16,r16,r27
8004bee4 A3 F0 00 00  lhz       r31,0x0(r16)                      =
8004bee8 3A 10 00 02  addi      r16,r16,0x2
8004beec A3 B0 00 00  lhz       r29,0x0(r16)                      = null
8004bef0 2C 1D 00 00  cmpwi     r29,0x0
8004bef4 41 82 00 54  beq       LAB_8004bf48
8004bef8 22 27 00 10  subfic    r17,r7,0x10
8004befc 8A 63 00 02  lbz       r19,0x2(lengths)
8004bf00 7E 73 88 30  slw       r19,r19,r17
8004bf04 22 27 00 08  subfic    r17,r7,0x8
8004bf08 8A 43 00 01  lbz       tbl6,0x1(lengths)
8004bf0c 7E 52 88 30  slw       tbl6,tbl6,r17
8004bf10 8A 23 00 00  lbz       r17,0x0(lengths)
8004bf14 5E 31 7E 3E  rlwnm     r17,r17,r15,0x18,0x1f
8004bf18 7E 31 93 78  or        r17,r17,tbl6
8004bf1c 7E 31 9B 78  or        r17,r17,r19
8004bf20 3A 40 00 01  li        tbl6,0x1
8004bf24 7E 52 E8 30  slw       tbl6,tbl6,r29
8004bf28 3A 52 FF FF  subi      tbl6,tbl6,0x1
8004bf2c 7E 30 90 38  and       r16,r17,tbl6
8004bf30 7F FF 82 14  add       r31,r31,r16
8004bf34 7C E7 EA 14  add       r7,r7,r29
8004bf38 7C F1 1E 70  srawi     r17,r7,0x3
8004bf3c 7C 63 8A 14  add       lengths,lengths,r17
8004bf40 70 E7 00 07  andi.     r7,r7,0x7
8004bf44 21 E7 00 20  subfic    r15,r7,0x20
LAB_8004bf48                       XREF[1]:   8004bef4(j)
8004bf48 7E FF 28 50  subf      r23,r31,pOut
8004bf4c 7F C9 03 A6  mtspr     CTR,r30
LAB_8004bf50                       XREF[1]:   8004bf58(j)
8004bf50 8E 17 00 01  lbzu      r16,0x1(r23)
8004bf54 9E 05 00 01  stbu      r16,0x1(pOut)
8004bf58 42 00 FF F8  bdnz      LAB_8004bf50
LAB_8004bf5c                       XREF[1]:   8004bde0(j)
8004bf5c 2C 19 01 00  cmpwi     r25,0x100
8004bf60 40 82 FD E8  bne       LAB_8004bd48
LAB_8004bf64                       XREF[2]:   8004b714(j), 8004bde4(j)
8004bf64 82 01 00 08  lwz       r16,0x8(r1)                       = null
8004bf68 2C 10 00 00  cmpwi     r16,0x0
8004bf6c 41 82 F7 0C  beq       LAB_8004b678
8004bf70 38 60 00 00  li        lengths,0x0
8004bf74 80 01 00 58  lwz       r0,0x58(r1)                       = 8000336Ch
8004bf78 7C 08 03 A6  mtspr     LR,r0
8004bf7c B9 C1 00 0C  lmw       r14,0xc(r1)                       = 8005C7C0h
                                              = 81700000h
                                              = 803f8450
                                              = 80003148h
                                              = 80020A9Ch
                                              = 803f8440
8004bf80 38 21 00 54  addi      r1,r1,offset DAT_803f8478
8004bf84 4E 80 00 20  blr
********** zlbDecompress Exit **********
