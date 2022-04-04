# GX Operations

Op|Name       |Params               |Description
--|-----------|---------------------|-----------
00|NOP        |none                 |No Operation
08|LOAD_CP_REG|rr vvvvvvvv          |CP REG rr = vvvvvvvv
10|LOAD_XF_REG|nnnn aaaa vvvvvvvv...|write n+1 u32 values to XF regs a, a+1...
20|LOAD_INDX_A|?                    |
28|LOAD_INDX_B|?                    |
30|LOAD_INDX_C|?                    |
38|LOAD_INDX_D|?                    |
40|CALL_DL    |aaaaaaaa ssssssss    |Call display list, address a, length s
48|INVAL_CACHE|?                    |Invalidate vertex cache
61|LOAD_BP_REG|vvvvvvvv             |write vvvvvvvv to BP ("SU_ByPassCmd")

## Drawing Commands
Opcodes 0x80 and above are drawing commands. The low 3 bits specify which VAT to use. The other bits specify the drawing mode.

Op|Name      |Description
--|----------|-----------
80|QUADS     |Draws pairs of tris sharing an edge (need not be coplanar)
88|QUADS2    |Same as QUADS, not normally used
90|TRIANGLES |Draw individual triangles
98|TRI_STRIP |Each triangle (after 1st) shares an edge with previous
A0|TRI_FAN   |Like TRI_STRIP but also sharing 1st vtx (eg a,b,c; a,c,d; a,d,e...)
A8|LINES     |Draw individual lines
B0|LINE_STRIP|Draw several connected vertices
B8|POINTS    |Draw individual points

After every draw opcode is number of vertices (u16) then vertex data.

### Vertex Data
For each vertex the fields are in a fixed order. The VCD tells whether each field is omitted, direct, or indexed; the VAT tells the data type of the field.

F#|Name      |Description
--|----------|------------
 0|PNMTXIDX  |Position/Normal Matrix Index
 1|TEX0MTXIDX|Texture 0 Matrix Index
 2|TEX1MTXIDX|Texture 1 Matrix Index
 3|TEX2MTXIDX|Texture 2 Matrix Index
 4|TEX3MTXIDX|Texture 3 Matrix Index
 5|TEX4MTXIDX|Texture 4 Matrix Index
 6|TEX5MTXIDX|Texture 5 Matrix Index
 7|TEX6MTXIDX|Texture 6 Matrix Index
 8|TEX7MTXIDX|Texture 7 Matrix Index
 9|POS       |Position Vector
10|NRM       |Normal or NBT - Binormal vector (T, B)
11|CLR0      |Color0 (Diffused)
12|CLR1      |Color1 (Specular)
13|TEX0      |Texture 0 data
14|TEX1      |Texture 1 data
15|TEX2      |Texture 2 data
16|TEX3      |Texture 3 data
17|TEX4      |Texture 4 data
18|TEX5      |Texture 5 data
19|TEX6      |Texture 6 data
20|TEX7      |Texture 7 data

Notice that the Position/Normal and Texture Matrix Indices are different from the other data in that they are 8 bit and must always be sent as direct data.

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

## VCD_LO
Bit|Size|Name    |Description
---|----|--------|-----------
  0|   1|PNMTXIDX|Position/Normal Matrix Index (*1)
  1|   1|T0MIDX  |Texture Coordinate 0 Matrix Index
  2|   1|T1MIDX  |Texture Coordinate 1 Matrix Index
  3|   1|T2MIDX  |Texture Coordinate 2 Matrix Index
  4|   1|T3MIDX  |Texture Coordinate 3 Matrix Index
  5|   1|T4MIDX  |Texture Coordinate 4 Matrix Index
  6|   1|T5MIDX  |Texture Coordinate 5 Matrix Index
  7|   1|T6MIDX  |Texture Coordinate 6 Matrix Index
  8|   1|T7MIDX  |Texture Coordinate 7 Matrix Index
  9|   2|POS     |Position value
 11|   2|NRM     |Normal or Normal/Binormal/Tangent
 13|   2|COL0    |Color0 (Diffused)
 15|   2|COL1    |Color1 (Specular)

(*1) position and normal matrices are stored in 2 seperate areas of
internal XF memory, but there is a one to one correspondence between
normal and position index. If index 'A' is used for the position,
then index 'A' needs to be used for the normal as well.

## VCD_HI
Bit|Size|Name    |Description
---|----|--------|-----------
  0|   2|TEX0    |Texture Coordinate 0 Value
  2|   2|TEX1    |Texture Coordinate 1 Value
  4|   2|TEX2    |Texture Coordinate 2 Value
  6|   2|TEX3    |Texture Coordinate 3 Value
  8|   2|TEX4    |Texture Coordinate 4 Value
 10|   2|TEX5    |Texture Coordinate 5 Value
 12|   2|TEX6    |Texture Coordinate 6 Value
 14|   2|TEX7    |Texture Coordinate 7 Value

VCD entries define the format of the field within the display list itself:
0: not present
1: direct (format specified by VAT)
2: u8 index
3: u16 index
Note, matrix indices are only 1 bit, so can't be indexed.

## VAT_A
Bit|Size|Name        |Description
---|----|------------|-----------
  0|   1|POSCNT      |Number of POS values
  1|   3|POSFMT      |Type of POS values
  4|   5|POSSHFT     |Shift of POS values
  9|   1|NRMCNT      |
 10|   3|NRMFMT      |
 13|   1|COL0CNT     |
 14|   3|COL0FMT     |
 17|   1|COL1CNT     |
 18|   3|COL1FMT     |
 21|   1|TEX0CNT     |
 22|   3|TEX0FMT     |
 25|   5|TEX0SHFT    |
 30|   1|BYTEDEQUANT |shift applies to u8/s8 (should always be 1)
 31|   1|NORMALINDEX3|1 or 3 idxs per normal

when nine-normals are selected in indirect mode, input will be
treated as three staggered indices (one per triple biased by
components size), into normal table (note: first index internally
biased by 0, second by 1, third by 2)

## VAT_B
Bit|Size|Name          |Description
---|----|--------------|-----------
  0|   1|TEX1CNT       |
  1|   3|TEX1FMT       |
  4|   5|TEX1SHFT      |
  9|   1|TEX2CNT       |
 10|   3|TEX2FMT       |
 13|   5|TEX2SHFT      |
 18|   1|TEX3CNT       |
 19|   3|TEX3FMT       |
 22|   5|TEX3SHFT      |
 27|   1|TEX4CNT       |
 28|   3|TEX4FMT       |
 31|   1|VCACHE_ENHANCE|must be 1 (or else ???)

## VAT_C
Bit|Size|Name        |Description
---|----|------------|-----------
  0|   5|TEX4SHFT    |
  5|   1|TEX5CNT     |
  6|   3|TEX5FMT     |
  9|   5|TEX5SHFT    |
 14|   1|TEX6CNT     |
 15|   3|TEX6FMT     |
 18|   5|TEX6SHFT    |
 23|   1|TEX7CNT     |
 24|   3|TEX7FMT     |
 27|   5|TEX7SHFT    |


# Display list processing
- Submit the CALL_DL opcode (0x40) followed by pointer to list (u32) and list length in bytes(?) (u32)
- Bytes are automatically read from the given address and handled the same as if they had been written to the FIFO manually, so you can have any opcode in a display list
    - Exception: you cannot call a list from another list
        - (what happens if you do? no idea)

Drawing:
- for n=0 to #vtxs:
    - read vtx according to VCD/VAT
    - draw vtx
notes:
    - VCD/VAT can never change during a draw op
    - there are 8 sets of VCDs and 8 sets of VATs
    - VCD and VAT are paired (if using VAT n, you're also using VCD n)
    - thus there are max 8 possible data formats at once
        - they can be changed between draw ops
    - vtx attr data can be in separate buffers or directly in the dlist
        - or omitted entirely
        - in both cases, can be in various formats (float, s16, s8...)
            - possible formats depend on the attribute

so for shaders we can use a few approaches:
- have them use one format and convert the buffers to that format when they're selected
    - similar to what we do now but without duplicating vtxs
    - only need to do it when first loading, not every frame
    - could have dlist parser just build buffers, not render them
- give them the raw buffer data and VAT/VCD data and let them read it as needed
    - complex
    - maybe slow
    - complicated with direct attrs (ie in the dlist itself)
        - can present the dlist itself as an interleaved buffer?
- bind several attrs to the same buffer with different types
    - eg: vtxpos_float, vtxpos_s16...
    - might be too many variations?

let's go with just converting everything to float
- what buffers does parser need to create/fill?
    - PNMTXIDX
    - TEXnMTXIDX (n=0..7)
    - POS
    - NRM
    - CLRn (n=0..1)
    - TEXn (n=0..7)
    - vtx index buffer (which vtxs to draw)
    - list of draw ops
        - eg: "3 triangles starting at entry 42 in the index buffer"

how should we handle direct attrs?
we can just append them to the data buffers but that kinda defeats the purpose of using indexed buffers...
could append them to separate data buffers, then concat them at the end, but that means going back through all the indices and adjusting them
since we know the formats we know whether the buffer data should be coming from an external buffer or the embedded params.
before we even execute the lists we must already know the size of each external buffer, so we can convert them ahead of time.

this is proving much more complicated than expected
if I just modify the GX sim to be able to return the generated buffers instead of rendering them it should have the same effect much more easily
but even that seems quite difficult
