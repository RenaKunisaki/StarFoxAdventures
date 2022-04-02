# GX Operations

Op|Name       |Params               |Description
--|-----------|---------------------|-----------
08|LOAD_CP_REG|rr vvvvvvvv          |CP REG rr = vvvvvvvv
10|LOAD_XF_REG|nnnn aaaa vvvvvvvv...|Write XF regs
61|LOAD_BP_REG|vvvvvvvv             |write vvvvvvvv to BP

## Write XF regs:
10 nnnn aaaa vvvvvvvv vvvvvvvv...
write nnnn+1 values (32 bits each) to XF regs starting at reg aaaa

# BP Regs
R#|Name               |Description
--|-------------------|-----------
00|GEN_MODE           |
01|display copy filter|
02|display copy filter|
03|display copy filter|
04|display copy filter|
05|?                  |
06|IND_MTXA0          |
07|IND_MTXB0          |
08|IND_MTXC0          |
09|IND_MTXA1          |
0A|IND_MTXB1          |
0B|IND_MTXC1          |
0C|IND_MTXA2          |
0D|IND_MTXB2          |
0E|IND_MTXC2          |
0F|IND_IMASK          |
10|IND_CMD0           |tev indirect 0
11|IND_CMD1           |tev indirect 1
12|IND_CMD2           |tev indirect 2
13|IND_CMD3           |tev indirect 3
14|IND_CMD4           |tev indirect 4
15|IND_CMD5           |tev indirect 5
16|IND_CMD6           |tev indirect 6
17|IND_CMD7           |tev indirect 7
18|IND_CMD8           |tev indirect 8
19|IND_CMD9           |tev indirect 9
1A|IND_CMDA           |tev indirect 10
1B|IND_CMDB           |tev indirect 11
1C|IND_CMDC           |tev indirect 12
1D|IND_CMDD           |tev indirect 13
1E|IND_CMDE           |tev indirect 14
1F|IND_CMDF           |tev indirect 15
20|scissor            |x0,y0 (0x20156156)
21|scissor            |x1,y1 (0x213d5335)
22|SU_LPSIZE          |field mode .. line width - point width
23|SU Counter (?)     |(0x23000000)
24|RAS Counter (?)    |(0x24000000)
25|RAS1_SS0           |ind tex coord scale 0
26|RAS1_SS1           |ind tex coord scale 1
27|RAS1_IREF          |
28|RAS1_TREF0         |tev order 0
29|RAS1_TREF1         |tev order 1
2A|RAS1_TREF2         |tev order 2
2B|RAS1_TREF3         |tev order 3
2C|RAS1_TREF4         |tev order 4
2D|RAS1_TREF5         |tev order 5
2E|RAS1_TREF6         |tev order 6
2F|RAS1_TREF7         |tev order 7
30|SU_SSIZE0          |texture offset 0 (Texture Size X, Y ?)
31|SU_TSIZE0          |
32|SU_SSIZE1          |texture offset 1
33|SU_TSIZE1          |
34|SU_SSIZE2          |texture offset 2
35|SU_TSIZE2          |
36|SU_SSIZE3          |texture offset 3
37|SU_TSIZE3          |
38|SU_SSIZE4          |texture offset 4
39|SU_TSIZE4          |
3A|SU_SSIZE5          |texture offset 5
3B|SU_TSIZE5          |
3C|SU_SSIZE6          |texture offset 6
3D|SU_TSIZE6          |
3E|SU_SSIZE7          |texture offset 7
3F|SU_TSIZE7          |
40|PE_ZMODE           |set z mode
41|PE_CMODE0          |dithering / blend mode/color_update/alpha_update/set_dither
42|PE_CMODE1          |destination alpha
43|PE_CONTROL         |comp z location z_comp_loc(0x43000040)pixel_fmt(0x43000041)
44|field mask         |(0x44000003)
45|PE_DONE            |draw done (end of list marker) ?
46|some clock ?       |(0x46000000 - (((162000000/500)/4224) - 0x0200))
47|PE_TOKEN           |token B (16 bit)
48|PE_TOKEN_INT       |token A (16 bit)
49|                   |EFB source rectangle top left
4A|                   |EFB source rectangle width, height-1
4B|                   |XFB target address
4C|?                  |
4D|stride ?           |
4E|DispCopyYScale     |
4F|PE copy clear AR   |set clear alpha and red components
50|PE copy clear GB   |green and blue
51|PE copy clear Z    |24-bit Z value
52|pe copy execute?   |
53|copy filter        |
54|copy filter        |
55|bounding box       |(0x550003ff)
56|bounding box       |(0x560003ff)
57|?                  |
58|?                  |(0x5800000f)
59|scissor-box offset |(0x5902acab)
5A|?                  |
5B|?                  |
5C|?                  |
5D|?                  |
5E|?                  |
5F|?                  |
60|?                  |
61|?                  |
62|?                  |
63|?                  |
64|TX_LOADTLUT0       |
65|TX_LOADTLUT1       |
66|?                  |
67|metric ?           |(0x67000000)
68|field mode         |
69|some clock ?       |(0x69000000 - ((((162000000/500)> >11)&0x00ffffff)) - 0x0400)
6A|?                  |
6B|?                  |
6C|?                  |
6D|?                  |
6E|?                  |
6F|?                  |
70|?                  |
71|?                  |
72|?                  |
73|?                  |
74|?                  |
75|?                  |
76|?                  |
77|?                  |
78|?                  |
79|?                  |
7A|?                  |
7B|?                  |
7C|?                  |
7D|?                  |
7E|?                  |
7F|?                  |
80|TX_SETMODE0_I0     |0x90 for linear
81|TX_SETMODE0_I1     |
82|TX_SETMODE0_I2     |
83|TX_SETMODE0_I3     |
84|TX_SETMODE1_I0     |
85|TX_SETMODE1_I1     |
86|TX_SETMODE1_I2     |
87|TX_SETMODE1_I3     |
88|TX_SETIMAGE0_I0    |texture size ?
89|TX_SETIMAGE0_I1    |
8A|TX_SETIMAGE0_I2    |
8B|TX_SETIMAGE0_I3    |
8C|TX_SETIMAGE1_I0    |
8D|TX_SETIMAGE1_I1    |
8E|TX_SETIMAGE1_I2    |
8F|TX_SETIMAGE1_I3    |
90|TX_SETIMAGE2_I0    |
91|TX_SETIMAGE2_I1    |
92|TX_SETIMAGE2_I2    |
93|TX_SETIMAGE2_I3    |
94|TX_SETIMAGE3_I0    |Texture Pointer
95|TX_SETIMAGE3_I1    |
96|TX_SETIMAGE3_I2    |
97|TX_SETIMAGE3_I3    |
98|TX_LOADTLUT0       |
99|TX_LOADTLUT1       |
9A|TX_LOADTLUT2       |
9B|TX_LOADTLUT3       |
9C|?                  |
9D|?                  |
9E|?                  |
9F|?                  |
A0|TX_SETMODE0_I4     |
A1|TX_SETMODE0_I5     |
A2|TX_SETMODE0_I6     |
A3|TX_SETMODE0_I7     |
A4|TX_SETMODE1_I4     |
A5|TX_SETMODE1_I5     |
A6|TX_SETMODE1_I6     |
A7|TX_SETMODE1_I7     |
A8|TX_SETIMAGE0_I4    |
A9|TX_SETIMAGE0_I5    |
AA|TX_SETIMAGE0_I6    |
AB|TX_SETIMAGE0_I7    |
AC|TX_SETIMAGE1_I4    |
AD|TX_SETIMAGE1_I5    |
AE|TX_SETIMAGE1_I6    |
AF|TX_SETIMAGE1_I7    |
B0|TX_SETIMAGE2_I4    |
B1|TX_SETIMAGE2_I5    |
B2|TX_SETIMAGE2_I6    |
B3|TX_SETIMAGE2_I7    |
B4|TX_SETIMAGE3_I4    |
B5|TX_SETIMAGE3_I5    |
B6|TX_SETIMAGE3_I6    |
B7|TX_SETIMAGE3_I7    |
B8|TX_SETTLUT_I4      |
B9|TX_SETTLUT_I5      |
BA|TX_SETTLUT_I6      |
BB|TX_SETTLUT_I7      |
BC|?                  |
BD|?                  |
BE|?                  |
BF|?                  |
C0|TEV_COLOR_ENV_0    |tev op 0
C1|TEV_ALPHA_ENV_0    |tev op 1
C2|TEV_COLOR_ENV_1    |
C3|TEV_ALPHA_ENV_1    |
C4|TEV_COLOR_ENV_2    |
C5|TEV_ALPHA_ENV_2    |
C6|TEV_COLOR_ENV_3    |
C7|TEV_ALPHA_ENV_3    |
C8|TEV_COLOR_ENV_4    |
C9|TEV_ALPHA_ENV_4    |
CA|TEV_COLOR_ENV_5    |
CB|TEV_ALPHA_ENV_5    |
CC|TEV_COLOR_ENV_6    |
CD|TEV_ALPHA_ENV_6    |
CE|TEV_COLOR_ENV_7    |
CF|TEV_ALPHA_ENV_7    |
D0|TEV_COLOR_ENV_8    |
D1|TEV_ALPHA_ENV_8    |
D2|TEV_COLOR_ENV_9    |
D3|TEV_ALPHA_ENV_9    |
D4|TEV_COLOR_ENV_A    |
D5|TEV_ALPHA_ENV_A    |
D6|TEV_COLOR_ENV_B    |
D7|TEV_ALPHA_ENV_B    |
D8|TEV_COLOR_ENV_C    |
D9|TEV_ALPHA_ENV_C    |
DA|TEV_COLOR_ENV_D    |
DB|TEV_ALPHA_ENV_D    |
DC|TEV_COLOR_ENV_E    |
DD|TEV_ALPHA_ENV_E    |
DE|TEV_COLOR_ENV_F    |
DF|TEV_ALPHA_ENV_F    |
E0|TEV_REGISTERL_0    |
E1|TEV_REGISTERH_0    |
E2|TEV_REGISTERL_1    |
E3|TEV_REGISTERH_1    |
E4|TEV_REGISTERL_2    |
E5|TEV_REGISTERH_2    |
E6|TEV_REGISTERL_3    |
E7|TEV_REGISTERH_3    |
E8|Fog Range          |(0xe8000156)
E9|?                  |
EA|?                  |
EB|?                  |
EC|?                  |(guessed: tev_range_adj_c)
ED|?                  |(guessed: tev_range_adj_k)
EE|TEV_FOG_PARAM_0    |(0xee03ce38)
EF|TEV_FOG_PARAM_1    |(0xef471c82)
F0|TEV_FOG_PARAM_2    |(0xf0000002)
F1|TEV_FOG_PARAM_3    |(0xf1000000)
F2|TEV_FOG_COLOR      |(0xf2000000)
F3|TEV_ALPHAFUNC      |alpha compare (0xf33f0000)
F4|TEV_Z_ENV_0        |z texture 0
F5|TEV_Z_ENV_1        |z texture 1
F6|TEV_KSEL_0         |Tev Swap Mode Table 0 (0xf6018064)
F7|TEV_KSEL_1         |Tev Swap Mode Table 1 (0xf701806e)
F8|TEV_KSEL_2         |Tev Swap Mode Table 2 (0xf8018060)
F9|TEV_KSEL_3         |Tev Swap Mode Table 3 (0xf901806c)
FA|TEV_KSEL_4         |Tev Swap Mode Table 4 (0xfa018065)
FB|TEV_KSEL_5         |Tev Swap Mode Table 5 (0xfb01806d)
FC|TEV_KSEL_6         |Tev Swap Mode Table 6 (0xfc01806a)
FD|TEV_KSEL_7         |Tev Swap Mode Table 7 (0xfd01806e)
FE|SS_MASK            |BP Mask Register
FF|?                  |

# CP Regs
R#|To|Name        |Description
--|--|------------|-----------
20|2F|?           |
30|33|MATINDEX_A  |Texture Matrix Index 0-3
40|43|MATINDEX_B  |Texture Matrix Index 4-7
50|57|VCD_LO      |Vertex Descriptor (VCD) low, format 0..7
60|67|VCD_HI      |Vertex Descriptor (VCD) high, format 0..7
70|77|VAT_A       |Vertex Attribute Table (VAT) group 0, format 0..7
80|87|VAT_B       |Vertex Attribute Table (VAT) group 1, format 0..7
90|97|VAT_C       |Vertex Attribute Table (VAT) group 2, format 0..7
A0|A3|ARRAY_BASE  |Vtxs, Normals, Color0, Color1
A4|AB|ARRAY_BASE  |TexCoord 0..7
AC|AF|ARRAY_BASE  |IndexRegA..D - general purpose array ptr
B0|BF|ARRAY_STRIDE|size of each array
