typedef unsigned char   undefined;

typedef unsigned char    bool;
typedef unsigned char    byte;
typedef unsigned int    dword;
typedef long long    longlong;
typedef unsigned int    uint;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    word;
#define A_ADPCM 1

#define A_AUX 8

#define A_CLEARBUFF 2

#define A_CONTINUE 0

#define A_DMEMMOVE 10

#define A_ENVMIXER 3

#define A_INIT 1

#define A_INTERLEAVE 13

#define A_LEFT 2

#define A_LOADADPCM 11

#define A_LOADBUFF 4

#define A_LOOP 2

#define A_MAIN 0

#define A_MIX 16

#define A_MIXER 12

#define A_NOAUX 0

#define A_OUT 2

#define A_POLEF 14

#define A_RATE 0

#define A_RESAMPLE 5

#define A_RIGHT 0

#define A_SAVEBUFF 6

#define A_SEGMENT 7

#define A_SETBUFF 8

#define A_SETLOOP 15

#define A_SETVOL 9

#define A_SPNOOP 0

#define A_VOL 4

#define ACMD_SIZE 32

typedef char sbyte;

typedef sbyte s8;

typedef struct CartOp CartOp, *PCartOp;

struct CartOp {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    char field_0x4;
    byte DOM1_LAT;
    byte DOM1_PGS;
    byte field_0x7;
    byte field_0x8;
    byte domain;
    undefined field_0xa;
    undefined field_0xb;
    uint cartAddr2;
};

typedef struct CartThreadCmd CartThreadCmd, *PCartThreadCmd;

typedef enum CartThreadOp {
    SaveWrite=15,
    RomRead=12,
    SaveRead=16,
    RomWrite=11,
    Nop=10
} CartThreadOp;

typedef struct OSMesgQueue OSMesgQueue, *POSMesgQueue;

typedef struct OSThread OSThread, *POSThread;

typedef enum OSThreadState {
    OS_STATE_RUNNABLE=2,
    OS_STATE_STOPPED=1,
    OS_STATE_RUNNING=4,
    OS_STATE_WAITING=8
} OSThreadState;

typedef ushort u16;

typedef struct __OSThreadContext __OSThreadContext, *P__OSThreadContext;

typedef ulonglong u64;

typedef uint u32;

typedef double f64;

struct __OSThreadContext {
    u64 at;
    u64 v0;
    u64 v1;
    u64 a0;
    u64 a1;
    u64 a2;
    u64 a3;
    u64 t0;
    u64 t1;
    u64 t2;
    u64 t3;
    u64 t4;
    u64 t5;
    u64 t6;
    u64 t7;
    u64 s0;
    u64 s1;
    u64 s2;
    u64 s3;
    u64 s4;
    u64 s5;
    u64 s6;
    u64 s7;
    u64 t8;
    u64 t9;
    u64 gp;
    u64 sp;
    u64 s8;
    u64 ra;
    u64 lo;
    u64 hi;
    u32 sr;
    u32 pc;
    u32 cause;
    u32 badvaddr;
    u32 rcp;
    u32 fpcsr;
    f64 fp0;
    f64 fp2;
    f64 fp4;
    f64 fp6;
    f64 fp8;
    f64 fp10;
    f64 fp12;
    f64 fp14;
    f64 fp16;
    f64 fp18;
    f64 fp20;
    f64 fp22;
    f64 fp24;
    f64 fp26;
    f64 fp28;
    f64 fp30;
};

struct OSThread {
    struct OSThread * next; /* run/mesg queue link */
    int priority; /* run/mesg queue priority */
    struct OSThread * * queue; /* queue thread is on */
    struct OSThread * tlnext; /* all threads queue link */
    enum OSThreadState state; /* OS_STATE_* */
    u16 flags; /* flags for rmon */
    int id; /* id for debugging */
    int fp; /* thread has used fp unit */
    struct __OSThreadContext context; /* register/interrupt mask */
};

struct OSMesgQueue {
    struct OSThread * mtqueue; /* Queue to store threads blocked on empty mailboxes (receive) */
    struct OSThread * fullqueue; /* Queue to store threads blocked on full mailboxes (send) */
    int validCount; /* Contains number of valid message */
    int first; /* Points to first valid message */
    int msgCount; /* Contains total # of messages */
    void * * msg; /* Points to message buffer array */
};

struct CartThreadCmd {
    enum CartThreadOp op;
    undefined field_0x2;
    undefined field_0x3;
    struct OSMesgQueue * respQueue;
    undefined4 ramAddr;
    undefined4 cartAddr;
    int length;
    struct CartOp * cartOp;
};

typedef struct CartThreadState CartThreadState, *PCartThreadState;

struct CartThreadState {
    int isInit;
    struct OSThread * thread;
    struct OSMesgQueue * queue;
    struct OSMesgQueue * piQueue;
    struct OSMesgQueue * queue10;
    undefined * accessCart;
    undefined * accessMaybeSavedata;
};

typedef struct DebugPacket DebugPacket, *PDebugPacket;

struct DebugPacket {
    int length;
    byte cmd;
    byte field_0x5;
    short field_0x6;
    byte field_0x8;
    byte isImem; /* 0=rmon 1=imem (DMA) */
    undefined field_0xa;
    undefined field_0xb;
    undefined4 id;
    void * data; /* from here on is probably command-specific format */
    uint len;
};

typedef struct DLL DLL, *PDLL;

struct DLL {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
};

typedef struct DllInfo DllInfo, *PDllInfo;

struct DllInfo {
    int id;
    int memSz;
    int field_0x8;
};

typedef struct LoadedDll LoadedDll, *PLoadedDll;

struct LoadedDll {
    int id; /* Created by retype action */
    int refCnt;
    pointer funcTbl;
    int size;
};

typedef struct RspThreadState RspThreadState, *PRspThreadState;

struct RspThreadState {
    byte field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    undefined field_0x8;
    undefined field_0x9;
    undefined field_0xa;
    undefined field_0xb;
    undefined field_0xc;
    undefined field_0xd;
    undefined field_0xe;
    undefined field_0xf;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
    undefined field_0x18;
    undefined field_0x19;
    undefined field_0x1a;
    undefined field_0x1b;
    undefined field_0x1c;
    undefined field_0x1d;
    undefined field_0x1e;
    undefined field_0x1f;
    undefined2 msg;
    undefined field_0x22;
    undefined field_0x23;
    undefined field_0x24;
    undefined field_0x25;
    undefined field_0x26;
    undefined field_0x27;
    undefined field_0x28;
    undefined field_0x29;
    undefined field_0x2a;
    undefined field_0x2b;
    undefined field_0x2c;
    undefined field_0x2d;
    undefined field_0x2e;
    undefined field_0x2f;
    undefined field_0x30;
    undefined field_0x31;
    undefined field_0x32;
    undefined field_0x33;
    undefined field_0x34;
    undefined field_0x35;
    undefined field_0x36;
    undefined field_0x37;
    undefined field_0x38;
    undefined field_0x39;
    undefined field_0x3a;
    undefined field_0x3b;
    undefined field_0x3c;
    undefined field_0x3d;
    undefined field_0x3e;
    undefined field_0x3f;
    int * queue;
    undefined field_0x44;
    undefined field_0x45;
    undefined field_0x46;
    undefined field_0x47;
    undefined field_0x48;
    undefined field_0x49;
    undefined field_0x4a;
    undefined field_0x4b;
    undefined field_0x4c;
    undefined field_0x4d;
    undefined field_0x4e;
    undefined field_0x4f;
    undefined field_0x50;
    undefined field_0x51;
    undefined field_0x52;
    undefined field_0x53;
    undefined field_0x54;
    undefined field_0x55;
    undefined field_0x56;
    undefined field_0x57;
    undefined2 msgs;
    undefined field_0x5a;
    undefined field_0x5b;
    undefined field_0x5c;
    undefined field_0x5d;
    undefined field_0x5e;
    undefined field_0x5f;
    undefined field_0x60;
    undefined field_0x61;
    undefined field_0x62;
    undefined field_0x63;
    undefined field_0x64;
    undefined field_0x65;
    undefined field_0x66;
    undefined field_0x67;
    undefined field_0x68;
    undefined field_0x69;
    undefined field_0x6a;
    undefined field_0x6b;
    undefined field_0x6c;
    undefined field_0x6d;
    undefined field_0x6e;
    undefined field_0x6f;
    undefined field_0x70;
    undefined field_0x71;
    undefined field_0x72;
    undefined field_0x73;
    undefined field_0x74;
    undefined field_0x75;
    undefined field_0x76;
    undefined field_0x77;
    undefined4 queue2;
    undefined field_0x7c;
    undefined field_0x7d;
    undefined field_0x7e;
    undefined field_0x7f;
    undefined field_0x80;
    undefined field_0x81;
    undefined field_0x82;
    undefined field_0x83;
    undefined field_0x84;
    undefined field_0x85;
    undefined field_0x86;
    undefined field_0x87;
    undefined field_0x88;
    undefined field_0x89;
    undefined field_0x8a;
    undefined field_0x8b;
    undefined field_0x8c;
    undefined field_0x8d;
    undefined field_0x8e;
    undefined field_0x8f;
    undefined2 msgs2;
    undefined field_0x92;
    undefined field_0x93;
    undefined field_0x94;
    undefined field_0x95;
    undefined field_0x96;
    undefined field_0x97;
    undefined field_0x98;
    undefined field_0x99;
    undefined field_0x9a;
    undefined field_0x9b;
    undefined field_0x9c;
    undefined field_0x9d;
    undefined field_0x9e;
    undefined field_0x9f;
    undefined field_0xa0;
    undefined field_0xa1;
    undefined field_0xa2;
    undefined field_0xa3;
    undefined field_0xa4;
    undefined field_0xa5;
    undefined field_0xa6;
    undefined field_0xa7;
    undefined field_0xa8;
    undefined field_0xa9;
    undefined field_0xaa;
    undefined field_0xab;
    undefined field_0xac;
    undefined field_0xad;
    undefined field_0xae;
    undefined field_0xaf;
    struct OSThread thread;
    undefined field_0x25c;
    undefined field_0x25d;
    undefined field_0x25e;
    undefined field_0x25f;
    undefined field_0x260;
    undefined field_0x261;
    undefined field_0x262;
    undefined field_0x263;
    undefined field_0x264;
    undefined field_0x265;
    undefined field_0x266;
    undefined field_0x267;
    undefined field_0x268;
    undefined field_0x269;
    undefined field_0x26a;
    undefined field_0x26b;
    undefined field_0x26c;
    undefined field_0x26d;
    undefined field_0x26e;
    undefined field_0x26f;
    undefined field_0x270;
    undefined field_0x271;
    undefined field_0x272;
    undefined field_0x273;
    undefined field_0x274;
    undefined field_0x275;
    undefined field_0x276;
    undefined field_0x277;
    undefined field_0x278;
    undefined field_0x279;
    undefined field_0x27a;
    undefined field_0x27b;
    undefined field_0x27c;
    undefined field_0x27d;
    undefined field_0x27e;
    undefined field_0x27f;
    undefined field_0x280;
    undefined field_0x281;
    undefined field_0x282;
    undefined field_0x283;
    undefined field_0x284;
    undefined field_0x285;
    undefined field_0x286;
    undefined field_0x287;
    undefined field_0x288;
    undefined field_0x289;
    undefined field_0x28a;
    undefined field_0x28b;
    undefined field_0x28c;
    undefined field_0x28d;
    undefined field_0x28e;
    undefined field_0x28f;
    undefined field_0x290;
    undefined field_0x291;
    undefined field_0x292;
    undefined field_0x293;
    undefined field_0x294;
    undefined field_0x295;
    undefined field_0x296;
    undefined field_0x297;
    undefined field_0x298;
    undefined field_0x299;
    undefined field_0x29a;
    undefined field_0x29b;
    undefined field_0x29c;
    undefined field_0x29d;
    undefined field_0x29e;
    undefined field_0x29f;
    undefined field_0x2a0;
    undefined field_0x2a1;
    undefined field_0x2a2;
    undefined field_0x2a3;
    undefined field_0x2a4;
    undefined field_0x2a5;
    undefined field_0x2a6;
    undefined field_0x2a7;
    undefined field_0x2a8;
    undefined field_0x2a9;
    undefined field_0x2aa;
    undefined field_0x2ab;
    undefined field_0x2ac;
    undefined field_0x2ad;
    undefined field_0x2ae;
    undefined field_0x2af;
    undefined field_0x2b0;
    undefined field_0x2b1;
    undefined field_0x2b2;
    undefined field_0x2b3;
    undefined field_0x2b4;
    undefined field_0x2b5;
    undefined field_0x2b6;
    undefined field_0x2b7;
    undefined field_0x2b8;
    undefined field_0x2b9;
    undefined field_0x2ba;
    undefined field_0x2bb;
    undefined field_0x2bc;
    undefined field_0x2bd;
    undefined field_0x2be;
    undefined field_0x2bf;
    undefined field_0x2c0;
    undefined field_0x2c1;
    undefined field_0x2c2;
    undefined field_0x2c3;
    undefined field_0x2c4;
    undefined field_0x2c5;
    undefined field_0x2c6;
    undefined field_0x2c7;
    undefined field_0x2c8;
    undefined field_0x2c9;
    undefined field_0x2ca;
    undefined field_0x2cb;
    undefined field_0x2cc;
    undefined field_0x2cd;
    undefined field_0x2ce;
    undefined field_0x2cf;
    undefined field_0x2d0;
    undefined field_0x2d1;
    undefined field_0x2d2;
    undefined field_0x2d3;
    undefined field_0x2d4;
    undefined field_0x2d5;
    undefined field_0x2d6;
    undefined field_0x2d7;
    undefined field_0x2d8;
    undefined field_0x2d9;
    undefined field_0x2da;
    undefined field_0x2db;
    undefined field_0x2dc;
    undefined field_0x2dd;
    undefined field_0x2de;
    undefined field_0x2df;
    undefined4 field_0x2e0;
    undefined4 field_0x2e4;
    undefined4 field_0x2e8;
    undefined4 field_0x2ec;
    undefined4 field_0x2f0;
    undefined4 field_0x2f4;
    undefined4 field_0x2f8;
    undefined4 field_0x2fc;
    undefined4 field_0x300;
};

typedef struct SiStruct1 SiStruct1, *PSiStruct1;

struct SiStruct1 {
    ushort field_0x0;
    byte field_0x2;
    byte field_0x3;
};

typedef struct ViThreadArg ViThreadArg, *PViThreadArg;

struct ViThreadArg {
    int isInit;
    struct OSThread * thisThread;
    struct OSMesgQueue * queue1;
    struct OSMesgQueue * queue2;
};

typedef struct OSContext OSContext, *POSContext;

struct OSContext {
    u32 gpr[32];
    u32 cr;
    u32 lr;
    u32 ctr;
    u32 xer;
    f64 fpr[32];
    u32 fpscr_pad;
    u32 fpscr;
    u32 srr0;
    u32 srr1;
    u16 mode;
    u16 state;
    u32 gqr[8];
    f64 psf[32];
};

typedef struct OSMutex OSMutex, *POSMutex;

typedef struct OSThreadQueue OSThreadQueue, *POSThreadQueue;


/* WARNING! conflicting data type names: /Dolphin/OS/OSThread.h/OSThread - /os.h/OSThread */

typedef int s32;

typedef struct OSMutexLink OSMutexLink, *POSMutexLink;

struct OSThreadQueue {
    struct OSThread * head;
    struct OSThread * tail;
};

struct OSMutexLink {
    struct OSMutex * next;
    struct OSMutex * prev;
};

struct OSMutex {
    struct OSThreadQueue queue;
    struct OSThread * thread;
    s32 count;
    struct OSMutexLink link;
};

typedef struct OSMutexQueue OSMutexQueue, *POSMutexQueue;

struct OSMutexQueue {
    struct OSMutex * head;
    struct OSMutex * tail;
};

typedef s32 OSPriority;

typedef struct OSThreadLink OSThreadLink, *POSThreadLink;

struct OSThreadLink {
    struct OSThread * next;
    struct OSThread * prev;
};

typedef byte u8;

#define AA_EN 8

#define ALPHA_CVG_SEL 8192

#define BOWTIE_VAL 0

#define CLR_ON_CVG 128

#define CVG_DST_CLAMP 0

#define CVG_DST_FULL 512

#define CVG_DST_SAVE 768

#define CVG_DST_WRAP 256

#define CVG_X_ALPHA 4096

#define FORCE_BL 16384

#define G_AC_DITHER 3

#define G_AC_NONE 0

#define G_AC_THRESHOLD 1

#define G_ACMUX_0 7

#define G_ACMUX_1 6

#define G_ACMUX_COMBINED 0

#define G_ACMUX_ENVIRONMENT 5

#define G_ACMUX_LOD_FRACTION 0

#define G_ACMUX_PRIM_LOD_FRAC 6

#define G_ACMUX_PRIMITIVE 3

#define G_ACMUX_SHADE 4

#define G_ACMUX_TEXEL0 1

#define G_ACMUX_TEXEL1 2

#define G_AD_DISABLE 48

#define G_AD_NOISE 32

#define G_AD_NOTPATTERN 16

#define G_AD_PATTERN 0

#define G_BL_0 3

#define G_BL_1 2

#define G_BL_1MA 0

#define G_BL_A_FOG 1

#define G_BL_A_IN 0

#define G_BL_A_MEM 1

#define G_BL_A_SHADE 2

#define G_BL_CLR_BL 2

#define G_BL_CLR_FOG 3

#define G_BL_CLR_IN 0

#define G_BL_CLR_MEM 1

#define G_CCMUX_0 31

#define G_CCMUX_1 6

#define G_CCMUX_CENTER 6

#define G_CCMUX_COMBINED 0

#define G_CCMUX_COMBINED_ALPHA 7

#define G_CCMUX_ENV_ALPHA 12

#define G_CCMUX_ENVIRONMENT 5

#define G_CCMUX_K4 7

#define G_CCMUX_K5 15

#define G_CCMUX_LOD_FRACTION 13

#define G_CCMUX_NOISE 7

#define G_CCMUX_PRIM_LOD_FRAC 14

#define G_CCMUX_PRIMITIVE 3

#define G_CCMUX_PRIMITIVE_ALPHA 10

#define G_CCMUX_SCALE 6

#define G_CCMUX_SHADE 4

#define G_CCMUX_SHADE_ALPHA 11

#define G_CCMUX_TEXEL0 1

#define G_CCMUX_TEXEL0_ALPHA 8

#define G_CCMUX_TEXEL1 2

#define G_CCMUX_TEXEL1_ALPHA 9

#define G_CD_BAYER 64

#define G_CD_DISABLE 192

#define G_CD_ENABLE 128

#define G_CD_MAGICSQ 0

#define G_CD_NOISE 128

#define G_CK_KEY 256

#define G_CK_NONE 0

#define G_CLEARGEOMETRYMODE -74

#define G_CLIPPING 0

#define G_CULL_BACK 8192

#define G_CULL_BOTH 12288

#define G_CULL_FRONT 4096

#define G_CULLDL -66

#define G_CV_K0 175

#define G_CV_K1 -43

#define G_CV_K2 -89

#define G_CV_K3 222

#define G_CV_K4 114

#define G_CV_K5 42

#define G_CYC_1CYCLE 0

#define G_CYC_2CYCLE 1048576

#define G_CYC_COPY 2097152

#define G_CYC_FILL 3145728

#define G_DL 6

#define G_DL_NOPUSH 1

#define G_DL_PUSH 0

#define G_DMACMDSIZ 128

#define G_ENDDL -72

#define G_FILLRECT 246

#define G_FOG 65536

#define G_IM_FMT_CI 2

#define G_IM_FMT_I 4

#define G_IM_FMT_IA 3

#define G_IM_FMT_RGBA 0

#define G_IM_FMT_YUV 1

#define G_IM_SIZ_16b 2

#define G_IM_SIZ_16b_BYTES 2

#define G_IM_SIZ_16b_INCR 0

#define G_IM_SIZ_16b_LINE_BYTES 2

#define G_IM_SIZ_16b_LOAD_BLOCK 2

#define G_IM_SIZ_16b_SHIFT 0

#define G_IM_SIZ_16b_TILE_BYTES 2

#define G_IM_SIZ_32b 3

#define G_IM_SIZ_32b_BYTES 4

#define G_IM_SIZ_32b_INCR 0

#define G_IM_SIZ_32b_LINE_BYTES 2

#define G_IM_SIZ_32b_LOAD_BLOCK 3

#define G_IM_SIZ_32b_SHIFT 0

#define G_IM_SIZ_32b_TILE_BYTES 2

#define G_IM_SIZ_4b 0

#define G_IM_SIZ_4b_BYTES 0

#define G_IM_SIZ_4b_INCR 3

#define G_IM_SIZ_4b_LINE_BYTES 0

#define G_IM_SIZ_4b_LOAD_BLOCK 2

#define G_IM_SIZ_4b_SHIFT 2

#define G_IM_SIZ_4b_TILE_BYTES 0

#define G_IM_SIZ_8b 1

#define G_IM_SIZ_8b_BYTES 1

#define G_IM_SIZ_8b_INCR 1

#define G_IM_SIZ_8b_LINE_BYTES 1

#define G_IM_SIZ_8b_LOAD_BLOCK 2

#define G_IM_SIZ_8b_SHIFT 1

#define G_IM_SIZ_8b_TILE_BYTES 1

#define G_IMMCMDSIZ 64

#define G_IMMFIRST -65

#define G_LIGHTING 131072

#define G_LINE3D -75

#define G_LOADBLOCK 243

#define G_LOADTILE 244

#define G_LOADTLUT 240

#define G_LOD 1048576

#define G_MAXFBZ 16383

#define G_MDSFT_ALPHACOMPARE 0

#define G_MDSFT_ALPHADITHER 4

#define G_MDSFT_BLENDER 16

#define G_MDSFT_BLENDMASK 0

#define G_MDSFT_COLORDITHER 22

#define G_MDSFT_COMBKEY 8

#define G_MDSFT_CYCLETYPE 20

#define G_MDSFT_PIPELINE 23

#define G_MDSFT_RENDERMODE 3

#define G_MDSFT_RGBDITHER 6

#define G_MDSFT_TEXTCONV 9

#define G_MDSFT_TEXTDETAIL 17

#define G_MDSFT_TEXTFILT 12

#define G_MDSFT_TEXTLOD 16

#define G_MDSFT_TEXTLUT 14

#define G_MDSFT_TEXTPERSP 19

#define G_MDSFT_ZSRCSEL 2

#define G_MOVEMEM 3

#define G_MOVEWORD -68

#define G_MTX 1

#define G_MTX_LOAD 2

#define G_MTX_MODELVIEW 0

#define G_MTX_MUL 0

#define G_MTX_NOPUSH 0

#define G_MTX_PROJECTION 1

#define G_MTX_PUSH 4

#define G_NOOP 192

#define G_PM_1PRIMITIVE 8388608

#define G_PM_NPRIMITIVE 0

#define G_POPMTX -67

#define G_RDP_ADDR_FIXUP 3

#define G_RDP_TRI_FILL_MASK 8

#define G_RDP_TRI_SHADE_MASK 4

#define G_RDP_TRI_TXTR_MASK 2

#define G_RDP_TRI_ZBUFF_MASK 1

#define G_RDPCMDSIZ 64

#define G_RDPFULLSYNC 233

#define G_RDPHALF_1 -76

#define G_RDPHALF_2 -77

#define G_RDPHALF_CONT -78

#define G_RDPLOADSYNC 230

#define G_RDPPIPESYNC 231

#define G_RDPSETOTHERMODE 239

#define G_RDPTILESYNC 232

#define G_RESERVED0 2

#define G_RESERVED1 5

#define G_RESERVED2 7

#define G_RESERVED3 8

#define G_RM_FOG_PRIM_A 3288334336

#define G_RM_FOG_SHADE_A 3355443200

#define G_RM_NOOP 0

#define G_RM_NOOP2 0

#define G_RM_PASS 201850880

#define G_ROTATE_FRAC 16

#define G_SC_EVEN_INTERLACE 2

#define G_SC_NON_INTERLACE 0

#define G_SC_ODD_INTERLACE 3

#define G_SCALE_FRAC 8

#define G_SETBLENDCOLOR 249

#define G_SETCIMG 255

#define G_SETCOMBINE 252

#define G_SETCONVERT 236

#define G_SETENVCOLOR 251

#define G_SETFILLCOLOR 247

#define G_SETFOGCOLOR 248

#define G_SETGEOMETRYMODE -73

#define G_SETKEYGB 234

#define G_SETKEYR 235

#define G_SETOTHERMODE_H -70

#define G_SETOTHERMODE_L -71

#define G_SETPRIMCOLOR 250

#define G_SETPRIMDEPTH 238

#define G_SETSCISSOR 237

#define G_SETTILE 245

#define G_SETTILESIZE 242

#define G_SETTIMG 253

#define G_SETZIMG 254

#define G_SHADE 4

#define G_SHADING_SMOOTH 512

#define G_SPNOOP 0

#define G_SPRITE2D_BASE 9

#define G_SPRITE2D_DRAW -67

#define G_SPRITE2D_SCALEFLIP -66

#define G_TC_CONV 0

#define G_TC_FILT 3072

#define G_TC_FILTCONV 2560

#define G_TD_CLAMP 0

#define G_TD_DETAIL 262144

#define G_TD_SHARPEN 131072

#define G_TEXRECT 228

#define G_TEXRECTFLIP 229

#define G_TEXTURE -69

#define G_TEXTURE_ENABLE 2

#define G_TEXTURE_GEN 262144

#define G_TEXTURE_GEN_LINEAR 524288

#define G_TEXTURE_IMAGE_FRAC 2

#define G_TEXTURE_SCALE_FRAC 16

#define G_TF_AVERAGE 12288

#define G_TF_BILERP 8192

#define G_TF_POINT 0

#define G_TL_LOD 65536

#define G_TL_TILE 0

#define G_TP_NONE 0

#define G_TP_PERSP 524288

#define G_TRI1 -65

#define G_TRI_FILL 200

#define G_TRI_FILL_ZBUFF 201

#define G_TRI_SHADE 204

#define G_TRI_SHADE_TXTR 206

#define G_TRI_SHADE_TXTR_ZBUFF 207

#define G_TRI_SHADE_ZBUFF 205

#define G_TRI_TXTR 202

#define G_TRI_TXTR_ZBUFF 203

#define G_TT_IA16 49152

#define G_TT_NONE 0

#define G_TT_RGBA16 32768

#define G_VTX 4

#define G_ZBUFFER 1

#define G_ZS_PIXEL 0

#define G_ZS_PRIM 4

#define IM_RD 64

#define TEX_EDGE 0

#define Z_CMP 16

#define Z_UPD 32

#define ZMODE_DEC 3072

#define ZMODE_INTER 1024

#define ZMODE_OPA 0

#define ZMODE_XLU 2048

#define GT_CULL_BACK 8192

#define GT_FLAG_NO_XFM 2

#define GT_FLAG_NOMTX 1

#define GT_FLAG_XFM_ONLY 4

#define GT_SHADING_SMOOTH 512

#define GT_TEXTURE 2

#define GT_ZBUFFER 1

#define FILTER_CLAMP 1

#define FILTER_WRAP 0

#define GU_BLINKRDP_EXTRACT 2

#define GU_BLINKRDP_HILITE 1

#define GU_PARSE_ABI_TYPE 5

#define GU_PARSE_GBI_TYPE 1

#define GU_PARSE_MEM_BLOCK 4

#define GU_PARSE_RDP_TYPE 2

#define GU_PARSE_READY 3

#define GU_PARSE_STRING_TYPE 6

#define GU_PARSEGBI_ALLMTX 16

#define GU_PARSEGBI_DUMPONLY 32

#define GU_PARSEGBI_FLTMTX 4

#define GU_PARSEGBI_NONEST 2

#define GU_PARSEGBI_ROWMAJOR 1

#define GU_PARSEGBI_SHOWDMA 8

#define GU_PARSERDP_DUMPONLY 32

#define GU_PARSERDP_PRAREA 2

#define GU_PARSERDP_PRHISTO 4

#define GU_PARSERDP_VERBOSE 1

#define ACTIVE 0

#define BLK_SIZE_ZONE0 19720

#define BLK_SIZE_ZONE1 18360

#define BLK_SIZE_ZONE2 17680

#define BLK_SIZE_ZONE3 16320

#define BLK_SIZE_ZONE4 14960

#define BLK_SIZE_ZONE5 13600

#define BLK_SIZE_ZONE6 12240

#define BLK_SIZE_ZONE7 10880

#define BLK_SIZE_ZONE8 9520

#define DDROM_FONT_START 655360

#define DDROM_WAVEDATA_START 1310720

#define LEO_ERROR_BUSY 8

#define LEO_ERROR_COMMAND_CLEARED_BY_HOST 34

#define LEO_ERROR_COMMAND_PHASE_ERROR 3

#define LEO_ERROR_COMMAND_TERMINATED 34

#define LEO_ERROR_DATA_PHASE_ERROR 4

#define LEO_ERROR_DEVICE_COMMUNICATION_FAILURE 41

#define LEO_ERROR_DIAGNOSTIC_FAILURE 2

#define LEO_ERROR_DIDNOT_CHANGED_DISK_AS_EXPECTED 46

#define LEO_ERROR_DISK_EJECTED_WHEN_ACCESSED 49

#define LEO_ERROR_DRIVE_NOT_READY 1

#define LEO_ERROR_GOOD 0

#define LEO_ERROR_INCOMPATIBLE_MEDIUM_INSTALLED 11

#define LEO_ERROR_INVALID_COMMAND_OPERATION_CODE 31

#define LEO_ERROR_LBA_OUT_OF_RANGE 32

#define LEO_ERROR_MEDIUM_MAY_HAVE_CHANGED 47

#define LEO_ERROR_MEDIUM_NOT_PRESENT 42

#define LEO_ERROR_NO_REFERENCE_POSITION_FOUND 24

#define LEO_ERROR_NO_SEEK_COMPLETE 21

#define LEO_ERROR_NOT_BOOTED_DISK 45

#define LEO_ERROR_POWERONRESET_DEVICERESET_OCCURED 43

#define LEO_ERROR_QUEUE_FULL 35

#define LEO_ERROR_RAMPACK_NOT_CONNECTED 44

#define LEO_ERROR_REAL_TIME_CLOCK_FAILURE 5

#define LEO_ERROR_RTC_NOT_SET_CORRECTLY 48

#define LEO_ERROR_TRACK_FOLLOWING_ERROR 25

#define LEO_ERROR_TRACKING_OR_SPDL_SERVO_FAILURE 25

#define LEO_ERROR_UNKNOWN_FORMAT 11

#define LEO_ERROR_UNRECOVERED_READ_ERROR 23

#define LEO_ERROR_WRITE_FAULT 22

#define LEO_ERROR_WRITE_PROTECT_ERROR 33

#define LEO_MOTOR_ACTIVE 0

#define LEO_MOTOR_SLEEP 2

#define LEO_MOTOR_STANDBY 1

#define LEO_SW_VERSION 1

#define MAX_BLK_SIZE 19720

#define MIN_BLK_SIZE 9520

#define NUM_LBAS 4292

#define OS_PRIORITY_LEOMGR 150

#define SLEEP 2

#define STANDBY 1

#define LEO_COMMAND_CLEAR_QUE 1

#define LEO_COMMAND_INQUIRY 2

#define LEO_COMMAND_MODE_SELECT 11

#define LEO_COMMAND_READ 5

#define LEO_COMMAND_READ_CAPACITY 9

#define LEO_COMMAND_READ_DISK_ID 12

#define LEO_COMMAND_READ_TIMER 13

#define LEO_COMMAND_REZERO 4

#define LEO_COMMAND_SEEK 7

#define LEO_COMMAND_SET_TIMER 14

#define LEO_COMMAND_START_STOP 8

#define LEO_COMMAND_TEST_UNIT_READY 3

#define LEO_COMMAND_TRANSLATE 10

#define LEO_COMMAND_WRITE 6

#define LEO_CONTROL_POST 128

#define LEO_CONTROL_START 1

#define LEO_CONTROL_STBY 2

#define LEO_CONTROL_TBL 1

#define LEO_CONTROL_WRT 1

#define LEO_DISK_TYPE_MAX 6

#define LEO_DISK_TYPE_MIN 0

#define LEO_LBA_MAX 4291

#define LEO_LBA_MIN 0

#define LEO_LBA_RAM_END6 4291

#define LEO_LBA_RAM_TOP0 1418

#define LEO_LBA_RAM_TOP1 1966

#define LEO_LBA_RAM_TOP2 2514

#define LEO_LBA_RAM_TOP3 3062

#define LEO_LBA_RAM_TOP4 3610

#define LEO_LBA_RAM_TOP5 4088

#define LEO_LBA_RAM_TOP6 4292

#define LEO_LBA_ROM_END0 1417

#define LEO_LBA_ROM_END1 1965

#define LEO_LBA_ROM_END2 2513

#define LEO_LBA_ROM_END3 3061

#define LEO_LBA_ROM_END4 3609

#define LEO_LBA_ROM_END5 4087

#define LEO_LBA_ROM_END6 4291

#define LEO_LBA_ROM_TOP 0

#define LEO_PRIORITY_INT 150

#define LEO_PRIORITY_WRK 149

#define LEO_SENSE_COMMAND_PHASE_ERROR 3

#define LEO_SENSE_COMMAND_TERMINATED 34

#define LEO_SENSE_DATA_PHASE_ERROR 4

#define LEO_SENSE_DEVICE_COMMUNICATION_FAILURE 41

#define LEO_SENSE_DIAGNOSTIC_FAILURE 2

#define LEO_SENSE_DRIVE_NOT_READY 1

#define LEO_SENSE_ILLEGAL_TIMER_VALUE 36

#define LEO_SENSE_INCOMPATIBLE_MEDIUM_INSTALLED 11

#define LEO_SENSE_INVALID_COMMAND_OPERATION_CODE 31

#define LEO_SENSE_LBA_OUT_OF_RANGE 32

#define LEO_SENSE_MEDIUM_MAY_HAVE_CHANGED 47

#define LEO_SENSE_MEDIUM_NOT_PRESENT 42

#define LEO_SENSE_NO_ADDITIONAL_SENSE_INFOMATION 0

#define LEO_SENSE_NO_REFERENCE_POSITION_FOUND 24

#define LEO_SENSE_NO_SEEK_COMPLETE 21

#define LEO_SENSE_POWERONRESET_DEVICERESET_OCCURED 43

#define LEO_SENSE_QUEUE_FULL 35

#define LEO_SENSE_REAL_TIME_CLOCK_FAILURE 5

#define LEO_SENSE_TRACK_FOLLOWING_ERROR 25

#define LEO_SENSE_TRACKING_OR_SPDL_SERVO_FAILURE 25

#define LEO_SENSE_UNKNOWN_FORMAT 11

#define LEO_SENSE_UNRECOVERED_READ_ERROR 23

#define LEO_SENSE_WRITE_FAULT 22

#define LEO_SENSE_WRITE_PROTECT_ERROR 33

#define LEO_STATUS_BUSY 8

#define LEO_STATUS_CHECK_CONDITION 2

#define LEO_STATUS_GOOD 0

#define LEO_TEST_UNIT_MR 1

#define LEO_TEST_UNIT_RE 2

#define LEO_TEST_UNIT_SS 4

#define AL_BANK_VERSION 16945

#define AL_CMIDI_BLOCK_CODE 254

#define AL_CMIDI_CNTRL_LOOPCOUNT_BIG 105

#define AL_CMIDI_CNTRL_LOOPCOUNT_SM 104

#define AL_CMIDI_CNTRL_LOOPEND 103

#define AL_CMIDI_CNTRL_LOOPSTART 102

#define AL_CMIDI_LOOPEND_CODE 45

#define AL_CMIDI_LOOPSTART_CODE 46

#define AL_DEFAULT_FXMIX 0

#define AL_DEFAULT_PRIORITY 5

#define AL_DEFAULT_VOICE 0

#define AL_EVTQ_END 2147483647

#define AL_FRAME_INIT -1

#define AL_FX_BIGROOM 2

#define AL_FX_BUFFER_SIZE 8192

#define AL_FX_CHORUS 3

#define AL_FX_CUSTOM 6

#define AL_FX_ECHO 5

#define AL_FX_FLANGE 4

#define AL_FX_NONE 0

#define AL_FX_SMALLROOM 1

#define AL_GAIN_CHANGE_TIME 1000

#define AL_HEAP_DEBUG 1

#define AL_HEAP_INIT 0

#define AL_HEAP_MAGIC 544434803

#define AL_KEY_MAX 127

#define AL_KEY_MIN 0

#define AL_MAX_CHANNELS 16

#define AL_MAX_PRIORITY 127

#define AL_PAN_CENTER 64

#define AL_PAN_LEFT 0

#define AL_PAN_RIGHT 127

#define AL_PHASE_ATTACK 0

#define AL_PHASE_DECAY 1

#define AL_PHASE_NOTEON 0

#define AL_PHASE_RELEASE 3

#define AL_PHASE_SUSTAIN 2

#define AL_PHASE_SUSTREL 4

#define AL_PLAYING 1

#define AL_STOPPED 0

#define AL_STOPPING 2

#define AL_SUSTAIN 63

#define AL_USEC_PER_FRAME 16000

#define AL_VOL_FULL 127

#define NO_SOUND_ERR_MASK 1

#define NO_VOICE_ERR_MASK 4

#define NOTE_OFF_ERR_MASK 2

#define FONTSIZE_E 2112

#define FONTSIZE_J 4768

#define G_OFF 0

#define G_ON 1

#define M_AUDTASK 2

#define M_GFXTASK 1

#define M_VIDTASK 3

#define NUM_SEGMENTS 16

#define __0 16

#define __1 17

#define __1QUOTE 55

#define __2 18

#define __2QUOTE 53

#define __3 19

#define __4 20

#define __5 21

#define __6 22

#define __7 23

#define __8 24

#define __9 25

#define __A 26

#define __ASTER 56

#define __AT 65

#define __B 27

#define __C 28

#define __COLON 62

#define __COMMA 58

#define __D 29

#define __E 30

#define __EQUAL 63

#define __EXCLAM 52

#define __F 31

#define __G 32

#define __H 33

#define __I 34

#define __J 35

#define __K 36

#define __L 37

#define __M 38

#define __MINUS 59

#define __N 39

#define __NUL 0

#define __NUMBR 54

#define __O 40

#define __P 41

#define __PERIOD 60

#define __PLUS 57

#define __Q 42

#define __QUEST 64

#define __R 43

#define __S 44

#define __SLASH 61

#define __SPC 15

#define __T 45

#define __U 46

#define __V 47

#define __W 48

#define __X 49

#define __Y 50

#define __Z 51

#define A_BUTTON 32768

#define B_BUTTON 16384

#define BLOCKSIZE 32

#define BOOT_ADDRESS_COSIM 2147491840

#define BOOT_ADDRESS_EMU 536936448

#define BOOT_ADDRESS_INDY 2282749952

#define BOOT_ADDRESS_ULTRA 2147484672

#define CONT_A 32768

#define CONT_ABSOLUTE 1

#define CONT_ADDR_CRC_ER 4

#define CONT_B 16384

#define CONT_C 2

#define CONT_CARD_ON 1

#define CONT_CARD_PULL 2

#define CONT_D 4

#define CONT_DOWN 1024

#define CONT_E 8

#define CONT_EEPROM 32768

#define CONT_EEPROM_BUSY 128

#define CONT_F 1

#define CONT_G 8192

#define CONT_JOYPORT 4

#define CONT_L 32

#define CONT_LEFT 512

#define CONT_NO_RESPONSE_ERROR 8

#define CONT_OVERRUN_ERROR 4

#define CONT_R 16

#define CONT_RELATIVE 2

#define CONT_RIGHT 256

#define CONT_START 4096

#define CONT_UP 2048

#define D_CBUTTONS 4

#define D_JPAD 1024

#define EEPROM_BLOCK_SIZE 8

#define EEPROM_MAXBLOCKS 64

#define L_CBUTTONS 2

#define L_JPAD 512

#define L_TRIG 32

#define LEO_BLOCK_MODE 1

#define LEO_SECTOR_MODE 3

#define LEO_TRACK_MODE 2

#define MAXCONTROLLERS 4

#define OS_APP_NMI_BUFSIZE 64

#define OS_EVENT_AI 6

#define OS_EVENT_CART 2

#define OS_EVENT_COUNTER 3

#define OS_EVENT_CPU_BREAK 10

#define OS_EVENT_DP 9

#define OS_EVENT_FAULT 12

#define OS_EVENT_PI 8

#define OS_EVENT_PRENMI 14

#define OS_EVENT_RDB_ACK_PROF 22

#define OS_EVENT_RDB_DATA_DONE 17

#define OS_EVENT_RDB_DBG_DONE 20

#define OS_EVENT_RDB_FLUSH_PROF 21

#define OS_EVENT_RDB_FREE_RAMROM 19

#define OS_EVENT_RDB_LOG_DONE 16

#define OS_EVENT_RDB_READ_DONE 15

#define OS_EVENT_RDB_REQ_RAMROM 18

#define OS_EVENT_SI 5

#define OS_EVENT_SP 4

#define OS_EVENT_SP_BREAK 11

#define OS_EVENT_SW1 0

#define OS_EVENT_SW2 1

#define OS_EVENT_THREADSTATUS 13

#define OS_EVENT_VI 7

#define OS_FLAG_CPU_BREAK 1

#define OS_FLAG_FAULT 2

#define OS_IM_AI 263169

#define OS_IM_ALL 4194049

#define OS_IM_CART 3073

#define OS_IM_COUNTER 33793

#define OS_IM_CPU 65281

#define OS_IM_DP 2098177

#define OS_IM_NONE 1

#define OS_IM_PI 1049601

#define OS_IM_PRENMI 5121

#define OS_IM_RDBREAD 17409

#define OS_IM_RDBWRITE 9217

#define OS_IM_SI 132097

#define OS_IM_SP 66561

#define OS_IM_SW1 1281

#define OS_IM_SW2 1537

#define OS_IM_VI 525313

#define OS_MESG_BLOCK 1

#define OS_MESG_NOBLOCK 0

#define OS_MESG_PRI_HIGH 1

#define OS_MESG_PRI_NORMAL 0

#define OS_MESG_TYPE_BASE 10

#define OS_MESG_TYPE_COUNTER 14

#define OS_MESG_TYPE_DMAREAD 11

#define OS_MESG_TYPE_DMAWRITE 12

#define OS_MESG_TYPE_EDMAREAD 15

#define OS_MESG_TYPE_EDMAWRITE 16

#define OS_MESG_TYPE_LOOPBACK 10

#define OS_MESG_TYPE_VRETRACE 13

#define OS_MIN_STACKSIZE 72

#define OS_NUM_EVENTS 23

#define OS_OTHERS 2

#define OS_PIM_STACKSIZE 4096

#define OS_PM_16K 24576

#define OS_PM_16M 33546240

#define OS_PM_1M 2088960

#define OS_PM_256K 516096

#define OS_PM_4K 0

#define OS_PM_4M 8380416

#define OS_PM_64K 122880

#define OS_PRIORITY_APPMAX 127

#define OS_PRIORITY_IDLE 0

#define OS_PRIORITY_MAX 255

#define OS_PRIORITY_PIMGR 150

#define OS_PRIORITY_RMON 250

#define OS_PRIORITY_RMONSPIN 200

#define OS_PRIORITY_SIMGR 140

#define OS_PRIORITY_VIMGR 254

#define OS_READ 0

#define OS_SIM_STACKSIZE 4096

#define OS_STATE_RUNNABLE 2

#define OS_STATE_RUNNING 4

#define OS_STATE_STOPPED 1

#define OS_STATE_WAITING 8

#define OS_TV_MPAL 2

#define OS_TV_NTSC 1

#define OS_TV_PAL 0

#define OS_VI_BIT_16PIXEL 64

#define OS_VI_BIT_32PIXEL 128

#define OS_VI_BIT_ANTIALIAS 16

#define OS_VI_BIT_DEFLICKINTERLACE 8

#define OS_VI_BIT_HIRES 512

#define OS_VI_BIT_INTERLACE 2

#define OS_VI_BIT_LORES 256

#define OS_VI_BIT_NONINTERLACE 1

#define OS_VI_BIT_NORMALINTERLACE 4

#define OS_VI_BIT_NTSC 1024

#define OS_VI_BIT_PAL 2048

#define OS_VI_BIT_POINTSAMPLE 32

#define OS_VI_DITHER_FILTER_OFF 128

#define OS_VI_DITHER_FILTER_ON 64

#define OS_VI_DIVOT_OFF 32

#define OS_VI_DIVOT_ON 16

#define OS_VI_GAMMA_DITHER_OFF 8

#define OS_VI_GAMMA_DITHER_ON 4

#define OS_VI_GAMMA_OFF 2

#define OS_VI_GAMMA_ON 1

#define OS_VI_MPAL_HAF1 39

#define OS_VI_MPAL_HAN1 38

#define OS_VI_MPAL_HPF1 37

#define OS_VI_MPAL_HPF2 41

#define OS_VI_MPAL_HPN1 36

#define OS_VI_MPAL_HPN2 40

#define OS_VI_MPAL_LAF1 31

#define OS_VI_MPAL_LAF2 35

#define OS_VI_MPAL_LAN1 30

#define OS_VI_MPAL_LAN2 34

#define OS_VI_MPAL_LPF1 29

#define OS_VI_MPAL_LPF2 33

#define OS_VI_MPAL_LPN1 28

#define OS_VI_MPAL_LPN2 32

#define OS_VI_NTSC_HAF1 11

#define OS_VI_NTSC_HAN1 10

#define OS_VI_NTSC_HPF1 9

#define OS_VI_NTSC_HPF2 13

#define OS_VI_NTSC_HPN1 8

#define OS_VI_NTSC_HPN2 12

#define OS_VI_NTSC_LAF1 3

#define OS_VI_NTSC_LAF2 7

#define OS_VI_NTSC_LAN1 2

#define OS_VI_NTSC_LAN2 6

#define OS_VI_NTSC_LPF1 1

#define OS_VI_NTSC_LPF2 5

#define OS_VI_NTSC_LPN1 0

#define OS_VI_NTSC_LPN2 4

#define OS_VI_PAL_HAF1 25

#define OS_VI_PAL_HAN1 24

#define OS_VI_PAL_HPF1 23

#define OS_VI_PAL_HPF2 27

#define OS_VI_PAL_HPN1 22

#define OS_VI_PAL_HPN2 26

#define OS_VI_PAL_LAF1 17

#define OS_VI_PAL_LAF2 21

#define OS_VI_PAL_LAN1 16

#define OS_VI_PAL_LAN2 20

#define OS_VI_PAL_LPF1 15

#define OS_VI_PAL_LPF2 19

#define OS_VI_PAL_LPN1 14

#define OS_VI_PAL_LPN2 18

#define OS_VIM_STACKSIZE 4096

#define OS_WRITE 1

#define PFS_CORRUPTED 2

#define PFS_CREATE 2

#define PFS_DATA_FULL 7

#define PFS_DIR_FULL 8

#define PFS_ERR_BAD_DATA 6

#define PFS_ERR_CONTRFAIL 4

#define PFS_ERR_DEVICE 11

#define PFS_ERR_EXIST 9

#define PFS_ERR_ID_FATAL 10

#define PFS_ERR_INCONSISTENT 3

#define PFS_ERR_INVALID 5

#define PFS_ERR_NEW_PACK 2

#define PFS_ERR_NOPACK 1

#define PFS_FILE_EXT_LEN 4

#define PFS_FILE_NAME_LEN 16

#define PFS_INITIALIZED 1

#define PFS_MAX_BANKS 62

#define PFS_ONE_PAGE 8

#define PFS_READ 0

#define PFS_WRITE 1

#define PROF_MIN_INTERVAL 50

#define R_CBUTTONS 1

#define R_JPAD 256

#define R_TRIG 16

#define RCP_IMASK 4128768

#define RCP_IMASKSHIFT 16

#define START_BUTTON 4096

#define U_CBUTTONS 8

#define U_JPAD 2048

#define Z_TRIG 8192

typedef enum OSEvent {
    OS_EVENT_RDB_ACK_PROF=22,
    OS_EVENT_COUNTER=3,
    OS_EVENT_RDB_DATA_DONE=17,
    OS_EVENT_RDB_FREE_RAMROM=19,
    OS_EVENT_RDB_DBG_DONE=20,
    OS_EVENT_SP_BREAK=11,
    OS_EVENT_SW1=0,
    OS_EVENT_RDB_READ_DONE=15,
    OS_EVENT_SW2=1,
    OS_EVENT_AI=6,
    OS_EVENT_DP=9,
    OS_EVENT_CPU_BREAK=10,
    OS_EVENT_VI=7,
    OS_EVENT_RDB_LOG_DONE=16,
    OS_EVENT_SI=5,
    OS_EVENT_FAULT=12,
    OS_EVENT_PRENMI=14,
    OS_EVENT_CART=2,
    OS_EVENT_RDB_REQ_RAMROM=18,
    OS_EVENT_RDB_FLUSH_PROF=21,
    OS_EVENT_PI=8,
    OS_EVENT_SP=4,
    OS_EVENT_THREADSTATUS=13
} OSEvent;

typedef enum OSEvent_times4 {
    OS_EVENT_RDB_ACK_PROF=88,
    OS_EVENT_RDB_DATA_DONE=68,
    OS_EVENT_COUNTER=12,
    OS_EVENT_RDB_FREE_RAMROM=76,
    OS_EVENT_RDB_DBG_DONE=80,
    OS_EVENT_SP_BREAK=44,
    OS_EVENT_SW1=0,
    OS_EVENT_RDB_READ_DONE=60,
    OS_EVENT_AI=24,
    OS_EVENT_SW2=4,
    OS_EVENT_DP=36,
    OS_EVENT_CPU_BREAK=40,
    OS_EVENT_RDB_LOG_DONE=64,
    OS_EVENT_VI=28,
    OS_EVENT_SI=20,
    OS_EVENT_PRENMI=56,
    OS_EVENT_FAULT=48,
    OS_EVENT_RDB_FLUSH_PROF=84,
    OS_EVENT_RDB_REQ_RAMROM=72,
    OS_EVENT_CART=8,
    OS_EVENT_PI=32,
    OS_EVENT_THREADSTATUS=52,
    OS_EVENT_SP=16
} OSEvent_times4;

#define BPPMASK 255

#define CM_COLORMAP 3

#define CM_DITHERED 1

#define CM_NORMAL 0

#define CM_SCREEN 2

#define IMAGIC 474

#define ITYPE_RLE 256

#define ITYPE_VERBATIM 0

#define RLE_NOP 0

#define TYPEMASK 65280

#define C0_BADVADDR 8

#define C0_CACHE_ERR 27

#define C0_CAUSE 13

#define C0_COMPARE 11

#define C0_CONFIG 16

#define C0_CONTEXT 4

#define C0_COUNT 9

#define C0_ECC 26

#define C0_ENTRYHI 10

#define C0_ENTRYLO0 2

#define C0_ENTRYLO1 3

#define C0_EPC 14

#define C0_ERROR_EPC 30

#define C0_IMPMASK 65280

#define C0_IMPSHIFT 8

#define C0_INX 0

#define C0_LLADDR 17

#define C0_MAJREVMASK 240

#define C0_MAJREVSHIFT 4

#define C0_MINREVMASK 15

#define C0_PAGEMASK 5

#define C0_PRID 15

#define C0_PROBE 8

#define C0_RAND 1

#define C0_READI 1

#define C0_REVMASK 255

#define C0_RFE 16

#define C0_SR 12

#define C0_TAGHI 29

#define C0_TAGLO 28

#define C0_WATCHHI 19

#define C0_WATCHLO 18

#define C0_WIRED 6

#define C0_WRITEI 2

#define C0_WRITER 6

#define C_CDX 12

#define C_FILL 20

#define C_HINV 16

#define C_HSV 28

#define C_HWB 24

#define C_HWBINV 20

#define C_IINV 0

#define C_ILT 4

#define C_IST 8

#define C_IWBINV 0

#define CACH_PD 1

#define CACH_PI 0

#define CACH_SD 3

#define CACH_SI 2

#define CACHERR_EB 33554432

#define CACHERR_EC 1073741824

#define CACHERR_ED 536870912

#define CACHERR_EE 67108864

#define CACHERR_EI 16777216

#define CACHERR_ER 2147483648

#define CACHERR_ES 134217728

#define CACHERR_ET 268435456

#define CACHERR_PIDX_MASK 7

#define CACHERR_PIDX_SHIFT 12

#define CACHERR_SIDX_MASK 4194296

#define CAUSE_BD 2147483648

#define CAUSE_CEMASK 805306368

#define CAUSE_CESHIFT 28

#define CAUSE_EXCMASK 124

#define CAUSE_EXCSHIFT 2

#define CAUSE_IP3 1024

#define CAUSE_IP4 2048

#define CAUSE_IP5 4096

#define CAUSE_IP6 8192

#define CAUSE_IP7 16384

#define CAUSE_IP8 32768

#define CAUSE_IPMASK 65280

#define CAUSE_IPSHIFT 8

#define CAUSE_SW1 256

#define CAUSE_SW2 512

#define CONFIG_BE 32768

#define CONFIG_BE_SHFT 15

#define CONFIG_CM 2147483648

#define CONFIG_COHRNT_EXLWR 5

#define CONFIG_CU 8

#define CONFIG_DB 16

#define CONFIG_DC 448

#define CONFIG_DC_SHFT 6

#define CONFIG_EB 8192

#define CONFIG_EC 1879048192

#define CONFIG_EC_1_1 6

#define CONFIG_EC_2_1 0

#define CONFIG_EC_3_1 1

#define CONFIG_EC_3_2 7

#define CONFIG_EM 16384

#define CONFIG_EP 251658240

#define CONFIG_EW 786432

#define CONFIG_IB 32

#define CONFIG_IC 3584

#define CONFIG_IC_SHFT 9

#define CONFIG_K0 7

#define CONFIG_NONCOHRNT 3

#define CONFIG_SB 12582912

#define CONFIG_SB_SHFT 22

#define CONFIG_SC 131072

#define CONFIG_SM 65536

#define CONFIG_SS 2097152

#define CONFIG_SW 1048576

#define CONFIG_UNCACHED 2

#define DCACHE_LINEMASK 15

#define DCACHE_LINESIZE 16

#define DCACHE_SIZE 8192

#define E_VEC 2147484032

#define ECC_VEC 2147483904

#define EXC_BREAK 36

#define EXC_CPU 44

#define EXC_DBE 28

#define EXC_FPE 60

#define EXC_IBE 24

#define EXC_II 40

#define EXC_INT 0

#define EXC_MOD 4

#define EXC_OV 48

#define EXC_RADE 16

#define EXC_RMISS 8

#define EXC_SYSCALL 32

#define EXC_TRAP 52

#define EXC_VCED 124

#define EXC_VCEI 56

#define EXC_WADE 20

#define EXC_WATCH 92

#define EXC_WMISS 12

#define FPCSR_C 8388608

#define FPCSR_CE 131072

#define FPCSR_CI 4096

#define FPCSR_CO 16384

#define FPCSR_CU 8192

#define FPCSR_CV 65536

#define FPCSR_CZ 32768

#define FPCSR_EI 128

#define FPCSR_EO 512

#define FPCSR_EU 256

#define FPCSR_EV 2048

#define FPCSR_EZ 1024

#define FPCSR_FI 4

#define FPCSR_FO 16

#define FPCSR_FS 16777216

#define FPCSR_FU 8

#define FPCSR_FV 64

#define FPCSR_FZ 32

#define FPCSR_RM_MASK 3

#define FPCSR_RM_RM 3

#define FPCSR_RM_RN 0

#define FPCSR_RM_RP 2

#define FPCSR_RM_RZ 1

#define ICACHE_LINEMASK 31

#define ICACHE_LINESIZE 32

#define ICACHE_SIZE 16384

#define K0BASE 2147483648

#define K0SIZE 536870912

#define K1BASE 2684354560

#define K1SIZE 536870912

#define K2BASE 3221225472

#define K2SIZE 536870912

#define KUBASE 0

#define KUSIZE 2147483648

#define NTLBENTRIES 31

#define PADDR_SHIFT 4

#define PADDRMASK 4294967040

#define PCLEANEXCL 128

#define PDIRTYEXCL 192

#define PINVALID 0

#define PPARITY_MASK 1

#define PSTATEMASK 192

#define R_VEC 3217031168

#define SADDR_SHIFT 4

#define SADDRMASK 4294959104

#define SCLEANEXCL 4096

#define SDIRTYEXCL 5120

#define SECC_MASK 127

#define SINVALID 0

#define SIZE_EXCVEC 128

#define SR_BEV 4194304

#define SR_CE 131072

#define SR_CH 262144

#define SR_CU0 268435456

#define SR_CU1 536870912

#define SR_CU2 1073741824

#define SR_CU3 2147483648

#define SR_CUMASK 4026531840

#define SR_DE 65536

#define SR_ERL 4

#define SR_EXL 2

#define SR_FR 67108864

#define SR_IBIT1 256

#define SR_IBIT2 512

#define SR_IBIT3 1024

#define SR_IBIT4 2048

#define SR_IBIT5 4096

#define SR_IBIT6 8192

#define SR_IBIT7 16384

#define SR_IBIT8 32768

#define SR_IE 1

#define SR_IMASK 65280

#define SR_IMASK0 65280

#define SR_IMASK1 65024

#define SR_IMASK2 64512

#define SR_IMASK3 63488

#define SR_IMASK4 61440

#define SR_IMASK5 57344

#define SR_IMASK6 49152

#define SR_IMASK7 32768

#define SR_IMASK8 0

#define SR_IMASKSHIFT 8

#define SR_ITS 16777216

#define SR_KSU_KER 0

#define SR_KSU_MASK 24

#define SR_KSU_SUP 8

#define SR_KSU_USR 16

#define SR_KX 128

#define SR_RE 33554432

#define SR_RP 134217728

#define SR_SR 1048576

#define SR_SX 64

#define SR_TS 2097152

#define SR_UX 32

#define SSTATEMASK 7168

#define SVINDEXMASK 896

#define TLBCTXT_BASEBITS 9

#define TLBCTXT_BASEMASK 4286578688

#define TLBCTXT_BASESHIFT 23

#define TLBCTXT_VPNMASK 8388592

#define TLBCTXT_VPNSHIFT 4

#define TLBHI_NPID 255

#define TLBHI_PIDMASK 255

#define TLBHI_PIDSHIFT 0

#define TLBHI_VPN2MASK 4294959104

#define TLBHI_VPN2SHIFT 13

#define TLBINX_INXMASK 63

#define TLBINX_INXSHIFT 0

#define TLBINX_PROBE 2147483648

#define TLBLO_CACHMASK 56

#define TLBLO_CACHSHIFT 3

#define TLBLO_D 4

#define TLBLO_EXLWR 40

#define TLBLO_G 1

#define TLBLO_NONCOHRNT 24

#define TLBLO_PFNMASK 1073741760

#define TLBLO_PFNSHIFT 6

#define TLBLO_UNCACHED 16

#define TLBLO_V 2

#define TLBPGMASK_16K 24576

#define TLBPGMASK_4K 0

#define TLBPGMASK_64K 122880

#define TLBRAND_RANDMASK 63

#define TLBRAND_RANDSHIFT 0

#define TLBWIRED_WIREDMASK 63

#define UT_VEC 2147483648

#define WATCHHI_VALIDMASK 15

#define WATCHLO_ADDRMASK 4294967288

#define WATCHLO_RTRAP 2

#define WATCHLO_VALIDMASK 4294967291

#define WATCHLO_WTRAP 1

#define XUT_VEC 2147483776

#define GAME_APP_DATA_READY 10

#define GAME_DBG_DATA_SEND 3

#define GAME_EXIT 16

#define GAME_FAULT_SEND 14

#define GAME_LOG_SEND 7

#define GAME_PRINTF_SEND 5

#define GAME_PROF_SEND 12

#define HOST_APP_CMD_READY 9

#define HOST_DATA_ACK 17

#define HOST_DBG_CMD_READY 2

#define HOST_DBG_DATA_ACK 4

#define HOST_FAULT_ACK 15

#define HOST_LOG_ACK 8

#define HOST_PIACCESS_REQ 1

#define HOST_PRINTF_ACK 6

#define HOST_PROF_ACK 13

#define HOST_PROF_REQ 11

#define RAMROM_APP_READ_ADDR 16752640

#define RAMROM_APP_WRITE_ADDR 16756736

#define RAMROM_BOOTADDR_OFFSET 8

#define RAMROM_BOOTSTRAP_OFFSET 64

#define RAMROM_BUF_SIZE 4096

#define RAMROM_CLOCKRATE_MASK 4294967280

#define RAMROM_CLOCKRATE_OFFSET 4

#define RAMROM_FONTDATA_OFFSET 2928

#define RAMROM_FONTDATA_SIZE 1152

#define RAMROM_GAME_OFFSET 4096

#define RAMROM_LOG_ADDR 16773120

#define RAMROM_MSG_ADDR 16752640

#define RAMROM_MSG_SIZE 24576

#define RAMROM_PIF2BOOTSTRAP_OFFSET 4096

#define RAMROM_PRINTF_ADDR 16769024

#define RAMROM_RELEASE_OFFSET 12

#define RAMROM_RMON_READ_ADDR 16760832

#define RAMROM_RMON_WRITE_ADDR 16764928

#define RAMROM_SIZE 16777216

#define AI_BASE_REG 72351744

#define AI_BITRATE_REG 72351764

#define AI_CONTROL_DMA_OFF 0

#define AI_CONTROL_DMA_ON 1

#define AI_CONTROL_REG 72351752

#define AI_DACRATE_REG 72351760

#define AI_DRAM_ADDR_REG 72351744

#define AI_LEN_REG 72351748

#define AI_MAX_BIT_RATE 16

#define AI_MAX_DAC_RATE 16384

#define AI_MIN_BIT_RATE 2

#define AI_MIN_DAC_RATE 132

#define AI_MPAL_MAX_FREQ 368000

#define AI_MPAL_MIN_FREQ 3000

#define AI_NTSC_MAX_FREQ 368000

#define AI_NTSC_MIN_FREQ 3000

#define AI_PAL_MAX_FREQ 376000

#define AI_PAL_MIN_FREQ 3050

#define AI_STATUS_DMA_BUSY 1073741824

#define AI_STATUS_FIFO_FULL 2147483648

#define AI_STATUS_REG 72351756

#define CHNL_ERR_COLLISION 64

#define CHNL_ERR_FRAME 128

#define CHNL_ERR_MASK 192

#define CHNL_ERR_NORESP 128

#define CHNL_ERR_OVERRUN 64

#define DEVICE_TYPE_64DD 2

#define DEVICE_TYPE_BULK 1

#define DEVICE_TYPE_CART 0

#define DPC_BASE_REG 68157440

#define DPC_BUFBUSY_REG 68157460

#define DPC_CLOCK_REG 68157456

#define DPC_CLR_CLOCK_CTR 512

#define DPC_CLR_CMD_CTR 256

#define DPC_CLR_FLUSH 16

#define DPC_CLR_FREEZE 4

#define DPC_CLR_PIPE_CTR 128

#define DPC_CLR_TMEM_CTR 64

#define DPC_CLR_XBUS_DMEM_DMA 1

#define DPC_CURRENT_REG 68157448

#define DPC_END_REG 68157444

#define DPC_PIPEBUSY_REG 68157464

#define DPC_SET_FLUSH 32

#define DPC_SET_FREEZE 8

#define DPC_SET_XBUS_DMEM_DMA 2

#define DPC_START_REG 68157440

#define DPC_STATUS_CBUF_READY 128

#define DPC_STATUS_CMD_BUSY 64

#define DPC_STATUS_DMA_BUSY 256

#define DPC_STATUS_END_VALID 512

#define DPC_STATUS_FLUSH 4

#define DPC_STATUS_FREEZE 2

#define DPC_STATUS_PIPE_BUSY 32

#define DPC_STATUS_REG 68157452

#define DPC_STATUS_START_GCLK 8

#define DPC_STATUS_START_VALID 1024

#define DPC_STATUS_TMEM_BUSY 16

#define DPC_STATUS_XBUS_DMEM_DMA 1

#define DPC_TMEM_REG 68157468

#define DPS_BASE_REG 69206016

#define DPS_BUFTEST_ADDR_REG 69206024

#define DPS_BUFTEST_DATA_REG 69206028

#define DPS_TBIST_CHECK 1

#define DPS_TBIST_CLEAR 4

#define DPS_TBIST_DONE 4

#define DPS_TBIST_FAILED 2040

#define DPS_TBIST_GO 2

#define DPS_TBIST_REG 69206016

#define DPS_TEST_MODE_REG 69206020

#define GIO_BASE_REG 402653184

#define GIO_CART_INTR_REG 402655232

#define GIO_GIO_INTR_REG 402653184

#define GIO_GIO_SYNC_REG 402654208

#define MI_BASE_REG 70254592

#define MI_CLR_DP_INTR 2048

#define MI_CLR_EBUS 512

#define MI_CLR_INIT 128

#define MI_CLR_RDRAM 4096

#define MI_INIT_MODE_REG 70254592

#define MI_INTR_AI 4

#define MI_INTR_DP 32

#define MI_INTR_MASK_AI 4

#define MI_INTR_MASK_CLR_AI 16

#define MI_INTR_MASK_CLR_DP 1024

#define MI_INTR_MASK_CLR_PI 256

#define MI_INTR_MASK_CLR_SI 4

#define MI_INTR_MASK_CLR_SP 1

#define MI_INTR_MASK_CLR_VI 64

#define MI_INTR_MASK_DP 32

#define MI_INTR_MASK_PI 16

#define MI_INTR_MASK_REG 70254604

#define MI_INTR_MASK_SET_AI 32

#define MI_INTR_MASK_SET_DP 2048

#define MI_INTR_MASK_SET_PI 512

#define MI_INTR_MASK_SET_SI 8

#define MI_INTR_MASK_SET_SP 2

#define MI_INTR_MASK_SET_VI 128

#define MI_INTR_MASK_SI 2

#define MI_INTR_MASK_SP 1

#define MI_INTR_MASK_VI 8

#define MI_INTR_PI 16

#define MI_INTR_REG 70254600

#define MI_INTR_SI 2

#define MI_INTR_SP 1

#define MI_INTR_VI 8

#define MI_MODE_EBUS 256

#define MI_MODE_INIT 128

#define MI_MODE_RDRAM 512

#define MI_MODE_REG 70254592

#define MI_NOOP_REG 70254596

#define MI_SET_EBUS 1024

#define MI_SET_INIT 256

#define MI_SET_RDRAM 8192

#define MI_VERSION_REG 70254596

#define PI_BASE_REG 73400320

#define PI_BSD_DOM1_LAT_REG 73400340

#define PI_BSD_DOM1_PGS_REG 73400348

#define PI_BSD_DOM1_PWD_REG 73400344

#define PI_BSD_DOM1_RLS_REG 73400352

#define PI_BSD_DOM2_LAT_REG 73400356

#define PI_BSD_DOM2_PGS_REG 73400364

#define PI_BSD_DOM2_PWD_REG 73400360

#define PI_BSD_DOM2_RLS_REG 73400368

#define PI_CART_ADDR_REG 73400324

#define PI_CLR_INTR 2

#define PI_DMA_BUFFER_SIZE 128

#define PI_DOM1_ADDR1 100663296

#define PI_DOM1_ADDR2 268435456

#define PI_DOM1_ADDR3 533725184

#define PI_DOM2_ADDR1 83886080

#define PI_DOM2_ADDR2 134217728

#define PI_DOM_LAT_OFS 0

#define PI_DOM_PGS_OFS 8

#define PI_DOM_PWD_OFS 4

#define PI_DOM_RLS_OFS 12

#define PI_DOMAIN1_REG 73400340

#define PI_DOMAIN2_REG 73400356

#define PI_DRAM_ADDR_REG 73400320

#define PI_RD_LEN_REG 73400328

#define PI_SET_RESET 1

#define PI_STATUS_CLR_INTR 2

#define PI_STATUS_DMA_BUSY 1

#define PI_STATUS_ERROR 4

#define PI_STATUS_IO_BUSY 2

#define PI_STATUS_REG 73400336

#define PI_STATUS_RESET 1

#define PI_WR_LEN_REG 73400332

#define PIF_RAM_END 532678655

#define PIF_RAM_START 532678592

#define PIF_ROM_END 532678591

#define PIF_ROM_START 532676608

#define RDRAM_0_BASE_ADDRESS 0

#define RDRAM_0_CONFIG 0

#define RDRAM_0_DEVICE_ID 0

#define RDRAM_0_END 2097151

#define RDRAM_0_START 0

#define RDRAM_1_BASE_ADDRESS 2097152

#define RDRAM_1_CONFIG 1024

#define RDRAM_1_DEVICE_ID 1

#define RDRAM_1_END 4194303

#define RDRAM_1_START 2097152

#define RDRAM_ACTIVE_MODE 1

#define RDRAM_ADDR_SELECT_REG 66060320

#define RDRAM_BASE_REG 66060288

#define RDRAM_CONFIG_REG 66060288

#define RDRAM_DELAY_REG 66060296

#define RDRAM_DEVICE_ID_REG 66060292

#define RDRAM_DEVICE_MANUF_REG 66060324

#define RDRAM_DEVICE_TYPE_REG 66060288

#define RDRAM_END 4194303

#define RDRAM_GLOBAL_CONFIG 524288

#define RDRAM_LENGTH 2097152

#define RDRAM_MIN_INTERVAL_REG 66060316

#define RDRAM_MODE_REG 66060300

#define RDRAM_RAS_INTERVAL_REG 66060312

#define RDRAM_REF_INTERVAL_REG 66060304

#define RDRAM_REF_ROW_REG 66060308

#define RDRAM_RESET_MODE 0

#define RDRAM_STANDBY_MODE 2

#define RDRAM_START 0

#define RI_BASE_REG 74448896

#define RI_CONFIG_REG 74448900

#define RI_COUNT_REG 74448912

#define RI_CURRENT_LOAD_REG 74448904

#define RI_LATENCY_REG 74448916

#define RI_MODE_REG 74448896

#define RI_REFRESH_REG 74448912

#define RI_RERROR_REG 74448920

#define RI_SELECT_REG 74448908

#define RI_WERROR_REG 74448924

#define SI_BASE_REG 75497472

#define SI_DRAM_ADDR_REG 75497472

#define SI_PIF_ADDR_RD64B_REG 75497476

#define SI_PIF_ADDR_WR64B_REG 75497488

#define SI_STATUS_DMA_BUSY 1

#define SI_STATUS_DMA_ERROR 8

#define SI_STATUS_INTERRUPT 4096

#define SI_STATUS_RD_BUSY 2

#define SI_STATUS_REG 75497496

#define SP_BASE_REG 67371008

#define SP_CLR_BROKE 4

#define SP_CLR_HALT 1

#define SP_CLR_INTR 8

#define SP_CLR_INTR_BREAK 128

#define SP_CLR_SIG0 512

#define SP_CLR_SIG1 2048

#define SP_CLR_SIG2 8192

#define SP_CLR_SIG3 32768

#define SP_CLR_SIG4 131072

#define SP_CLR_SIG5 524288

#define SP_CLR_SIG6 2097152

#define SP_CLR_SIG7 8388608

#define SP_CLR_SSTEP 32

#define SP_CLR_TASKDONE 8192

#define SP_CLR_YIELD 512

#define SP_CLR_YIELDED 2048

#define SP_DMA_BUSY_REG 67371032

#define SP_DMA_DMEM 0

#define SP_DMA_FULL_REG 67371028

#define SP_DMA_IMEM 4096

#define SP_DMEM_END 67112959

#define SP_DMEM_START 67108864

#define SP_DRAM_ADDR_REG 67371012

#define SP_IBIST_CHECK 1

#define SP_IBIST_CLEAR 4

#define SP_IBIST_DONE 4

#define SP_IBIST_FAILED 120

#define SP_IBIST_GO 2

#define SP_IBIST_REG 67633156

#define SP_IMEM_END 67117055

#define SP_IMEM_START 67112960

#define SP_MEM_ADDR_REG 67371008

#define SP_PC_REG 67633152

#define SP_RD_LEN_REG 67371016

#define SP_SEMAPHORE_REG 67371036

#define SP_SET_HALT 2

#define SP_SET_INTR 16

#define SP_SET_INTR_BREAK 256

#define SP_SET_SIG0 1024

#define SP_SET_SIG1 4096

#define SP_SET_SIG2 16384

#define SP_SET_SIG3 65536

#define SP_SET_SIG4 262144

#define SP_SET_SIG5 1048576

#define SP_SET_SIG6 4194304

#define SP_SET_SIG7 16777216

#define SP_SET_SSTEP 64

#define SP_SET_TASKDONE 16384

#define SP_SET_YIELD 1024

#define SP_SET_YIELDED 4096

#define SP_STATUS_BROKE 2

#define SP_STATUS_DMA_BUSY 4

#define SP_STATUS_DMA_FULL 8

#define SP_STATUS_HALT 1

#define SP_STATUS_INTR_BREAK 64

#define SP_STATUS_IO_FULL 16

#define SP_STATUS_REG 67371024

#define SP_STATUS_SIG0 128

#define SP_STATUS_SIG1 256

#define SP_STATUS_SIG2 512

#define SP_STATUS_SIG3 1024

#define SP_STATUS_SIG4 2048

#define SP_STATUS_SIG5 4096

#define SP_STATUS_SIG6 8192

#define SP_STATUS_SIG7 16384

#define SP_STATUS_SSTEP 32

#define SP_STATUS_TASKDONE 512

#define SP_STATUS_YIELD 128

#define SP_STATUS_YIELDED 256

#define SP_WR_LEN_REG 67371020

#define VI_BASE_REG 71303168

#define VI_BURST_REG 71303188

#define VI_CONTROL_REG 71303168

#define VI_CTRL_ANTIALIAS_MASK 768

#define VI_CTRL_DITHER_FILTER_ON 65536

#define VI_CTRL_DIVOT_ON 16

#define VI_CTRL_GAMMA_DITHER_ON 4

#define VI_CTRL_GAMMA_ON 8

#define VI_CTRL_SERRATE_ON 64

#define VI_CTRL_TYPE_16 2

#define VI_CTRL_TYPE_32 3

#define VI_CURRENT_REG 71303184

#define VI_DRAM_ADDR_REG 71303172

#define VI_H_START_REG 71303204

#define VI_H_SYNC_LEAP_REG 71303200

#define VI_H_SYNC_REG 71303196

#define VI_H_VIDEO_REG 71303204

#define VI_H_WIDTH_REG 71303176

#define VI_INTR_REG 71303180

#define VI_LEAP_REG 71303200

#define VI_MPAL_CLOCK 48628316

#define VI_NTSC_CLOCK 48681812

#define VI_ORIGIN_REG 71303172

#define VI_PAL_CLOCK 49656530

#define VI_STATUS_REG 71303168

#define VI_TIMING_REG 71303188

#define VI_V_BURST_REG 71303212

#define VI_V_CURRENT_LINE_REG 71303184

#define VI_V_INTR_REG 71303180

#define VI_V_START_REG 71303208

#define VI_V_SYNC_REG 71303192

#define VI_V_VIDEO_REG 71303208

#define VI_WIDTH_REG 71303176

#define VI_X_SCALE_REG 71303216

#define VI_Y_SCALE_REG 71303220

#define DEBUG_COMMAND_INVALID 255

#define DEBUG_COMMAND_MEMORY 1

#define DEBUG_COMMAND_NULL 0

#define DEBUG_COMMAND_REGISTER 2

#define DEBUG_STATE_INVALID 255

#define DEBUG_STATE_NULL 0

#define DEBUG_STATE_RECEIVE 1

#define GIO_RDB_BASE_REG 3209166848

#define GIO_RDB_DEBUG_MINOR 2

#define GIO_RDB_PRINT_MINOR 1

#define GIO_RDB_READ_INTR_BIT 1073741824

#define GIO_RDB_READ_INTR_REG 3209166860

#define GIO_RDB_WRITE_INTR_BIT 2147483648

#define GIO_RDB_WRITE_INTR_REG 3209166856

#define PROF_BLOCK_SIZE 2048

#define RDB_BASE_REG 3221225472

#define RDB_BASE_VIRTUAL_ADDR 2147483648

#define RDB_DATA_MAX_BLOCK_SIZE 32768

#define RDB_LOG_MAX_BLOCK_SIZE 32768

#define RDB_PROF_ACK_SIG 1

#define RDB_PROF_FLUSH_SIG 2

#define RDB_READ_INTR_REG 3221225484

#define RDB_TYPE_GtoH_DATA 7

#define RDB_TYPE_GtoH_DATA_CT 6

#define RDB_TYPE_GtoH_DEBUG 8

#define RDB_TYPE_GtoH_DEBUG_DONE 10

#define RDB_TYPE_GtoH_DEBUG_READY 11

#define RDB_TYPE_GtoH_FAULT 2

#define RDB_TYPE_GtoH_KDEBUG 12

#define RDB_TYPE_GtoH_LOG 4

#define RDB_TYPE_GtoH_LOG_CT 3

#define RDB_TYPE_GtoH_PRINT 1

#define RDB_TYPE_GtoH_PROF_DATA 22

#define RDB_TYPE_GtoH_RAMROM 9

#define RDB_TYPE_GtoH_READY_FOR_DATA 5

#define RDB_TYPE_HtoG_DATA 16

#define RDB_TYPE_HtoG_DATA_DONE 17

#define RDB_TYPE_HtoG_DEBUG 14

#define RDB_TYPE_HtoG_DEBUG_CT 15

#define RDB_TYPE_HtoG_FREE_RAMROM 19

#define RDB_TYPE_HtoG_KDEBUG 20

#define RDB_TYPE_HtoG_LOG_DONE 13

#define RDB_TYPE_HtoG_PROF_SIGNAL 21

#define RDB_TYPE_HtoG_REQ_RAMROM 18

#define RDB_TYPE_INVALID 0

#define RDB_WRITE_INTR_REG 3221225480

#define BUF_FREE_WO_NEXT 32768

#define MAX_BUFCOUNT 32768

#define OS_RG_ALIGN_16B 16

#define OS_RG_ALIGN_2B 2

#define OS_RG_ALIGN_4B 4

#define OS_RG_ALIGN_8B 8

#define OS_RG_ALIGN_DEFAULT 8

#define RMON_DBG_BUF_SIZE 2048

#define RMON_STACKSIZE 4096

#define OS_SC_DONE_MSG 2

#define OS_SC_DRAM_DLIST 4

#define OS_SC_LAST_MSG 4

#define OS_SC_LAST_TASK 32

#define OS_SC_MAX_MESGS 8

#define OS_SC_NEEDS_RDP 1

#define OS_SC_NEEDS_RSP 2

#define OS_SC_PARALLEL_TASK 16

#define OS_SC_PRE_NMI_MSG 4

#define OS_SC_RCP_MASK 3

#define OS_SC_RDP_DONE_MSG 3

#define OS_SC_RETRACE_MSG 1

#define OS_SC_STACKSIZE 8192

#define OS_SC_SWAPBUFFER 64

#define OS_SC_TYPE_MASK 7

#define DL_BM_OVERHEAD 12

#define DL_SPRITE_OVERHEAD 24

#define SP_CUTOUT 2

#define SP_EXTERN 1024

#define SP_FASTCOPY 32

#define SP_FRACPOS 256

#define SP_HIDDEN 4

#define SP_OVERLAP 64

#define SP_SCALE 16

#define SP_TEXSHIFT 128

#define SP_TEXSHUF 512

#define SP_TRANSPARENT 1

#define SP_Z 8

#define OS_TASK_DP_WAIT 2

#define OS_TASK_LOADABLE 4

#define OS_TASK_USR0 16

#define OS_TASK_USR1 32

#define OS_TASK_USR2 64

#define OS_TASK_USR3 128

#define OS_TASK_YIELDED 1

#define OS_YIELD_AUDIO_SIZE 1024

#define OS_YIELD_DATA_SIZE 2304

#define DOMAIN 1

#define OVERFLOW 3

#define PLOSS 6

#define SING 2

#define TLOSS 5

#define UNDERFLOW 4

#define CART_INTR_REG_OFFSET 2048

#define DG_CARTREG_MASK 63

#define DG_RAMROM_CMD_DONE 4

#define DG_RAMROM_CMD_READY 3

#define DG_RAMROM_GRANT 2

#define DG_RAMROM_REQUEST 1

#define DG_TINYMON_DMA_READ_OP 3

#define DG_TINYMON_DMA_WRITE_OP 4

#define DG_TINYMON_READ_OP 1

#define DG_TINYMON_WRITE_OP 2

#define GIO_BASE_REG_UPPER 6144

#define GIO_BASE_REG_UPPER_K1 47104

#define GIO_INTR_REG_OFFSET 0

#define GIO_SYNC_REG_OFFSET 1024

#define PI_BASE_REG_UPPER 1120

#define PI_BASE_REG_UPPER_K1 42080

#define PI_STATUS_REG_OFFSET 16

#define PIFMON_WRITE_ADDR 264241200

#define RAMROM_BASE_UPPER 4096

#define RAMROM_BASE_UPPER_K1 45056

#define _U64_PRODUCT_ID_MASK 127

#define _U64_PRODUCT_ID_VALUE 21

#define _U64_REGMASK 31

#define _U64_RESET_CONTROL_NMI 4

#define _U64_RESET_CONTROL_RESET 2

#define U64_GIOBUS_BASE 524288000

#define U64_GIOBUS_SIZE 2097152

#define U64_MEM_SIZE 1048576

#define ERR_ALBNKFNEW 113

#define ERR_ALCSEQZEROSTATUS 128

#define ERR_ALCSEQZEROVEL 129

#define ERR_ALCSPVNOTFREE 130

#define ERR_ALEVENTNOFREE 124

#define ERR_ALHEAPCORRUPT 126

#define ERR_ALHEAPFIRSTBLOCK 127

#define ERR_ALHEAPNOFREE 125

#define ERR_ALMODDELAYOVERFLOW 133

#define ERR_ALSEQMETA 120

#define ERR_ALSEQNOTMIDI 114

#define ERR_ALSEQNOTMIDI0 115

#define ERR_ALSEQNUMTRACKS 116

#define ERR_ALSEQOVERRUN 131

#define ERR_ALSEQP_MAP_VOICE 102

#define ERR_ALSEQP_NO_SOUND 100

#define ERR_ALSEQP_NO_VOICE 101

#define ERR_ALSEQP_OFF_VOICE 103

#define ERR_ALSEQP_POLY_VOICE 104

#define ERR_ALSEQPINVALIDPROG 121

#define ERR_ALSEQPUNKNOWNMIDI 122

#define ERR_ALSEQPUNMAP 123

#define ERR_ALSEQSYSEX 119

#define ERR_ALSEQTIME 117

#define ERR_ALSEQTRACKHDR 118

#define ERR_ALSNDP_NO_VOICE 105

#define ERR_ALSNDPDEALLOCATE 107

#define ERR_ALSNDPDELETE 108

#define ERR_ALSNDPPLAY 109

#define ERR_ALSNDPSETPAR 112

#define ERR_ALSNDPSETPRIORITY 111

#define ERR_ALSNDPSETSOUND 110

#define ERR_ALSYN_NO_UPDATE 106

#define ERR_OSAISETFREQUENCY 14

#define ERR_OSAISETNEXTBUFFER_ADDR 15

#define ERR_OSAISETNEXTBUFFER_ENDADDR 132

#define ERR_OSAISETNEXTBUFFER_SIZE 16

#define ERR_OSCREATEMESGQUEUE 5

#define ERR_OSCREATEPIMANAGER 35

#define ERR_OSCREATEREGION_ALIGN 50

#define ERR_OSCREATEREGION_SIZE 51

#define ERR_OSCREATETHREAD_PRI 2

#define ERR_OSCREATETHREAD_SP 1

#define ERR_OSCREATEVIMANAGER 49

#define ERR_OSDPSETNEXTBUFFER_ADDR 17

#define ERR_OSDPSETNEXTBUFFER_SIZE 18

#define ERR_OSFREE_ADDR 54

#define ERR_OSFREE_REGION 53

#define ERR_OSGETREGIONBUFCOUNT 55

#define ERR_OSGETREGIONBUFSIZE 56

#define ERR_OSGETTIME 74

#define ERR_OSJAMMESG 7

#define ERR_OSMALLOC 52

#define ERR_OSMAPTLB_ASID 11

#define ERR_OSMAPTLB_INDEX 10

#define ERR_OSPIRAWREADIO 19

#define ERR_OSPIRAWSTARTDMA_ADDR 23

#define ERR_OSPIRAWSTARTDMA_DEVADDR 22

#define ERR_OSPIRAWSTARTDMA_DIR 21

#define ERR_OSPIRAWSTARTDMA_RANGE 25

#define ERR_OSPIRAWSTARTDMA_SIZE 24

#define ERR_OSPIRAWWRITEIO 20

#define ERR_OSPIREADIO 26

#define ERR_OSPISTARTDMA_ADDR 32

#define ERR_OSPISTARTDMA_DEVADDR 31

#define ERR_OSPISTARTDMA_DIR 30

#define ERR_OSPISTARTDMA_PIMGR 28

#define ERR_OSPISTARTDMA_PRI 29

#define ERR_OSPISTARTDMA_RANGE 34

#define ERR_OSPISTARTDMA_SIZE 33

#define ERR_OSPIWRITEIO 27

#define ERR_OSPROFILEINIT_ALN 63

#define ERR_OSPROFILEINIT_CNT 62

#define ERR_OSPROFILEINIT_ORD 64

#define ERR_OSPROFILEINIT_SIZ 65

#define ERR_OSPROFILEINIT_STR 61

#define ERR_OSPROFILESTART_FLAG 67

#define ERR_OSPROFILESTART_TIME 66

#define ERR_OSPROFILESTOP_FLAG 68

#define ERR_OSPROFILESTOP_TIMER 69

#define ERR_OSREADHOST_ADDR 70

#define ERR_OSREADHOST_SIZE 71

#define ERR_OSRECVMESG 8

#define ERR_OSSENDMESG 6

#define ERR_OSSETEVENTMESG 9

#define ERR_OSSETTHREADPRI 4

#define ERR_OSSETTIME 75

#define ERR_OSSETTIMER 76

#define ERR_OSSETTLBASID 13

#define ERR_OSSPTASKLOAD_DRAM 57

#define ERR_OSSPTASKLOAD_OUT 58

#define ERR_OSSPTASKLOAD_OUTSIZE 59

#define ERR_OSSPTASKLOAD_YIELD 60

#define ERR_OSSTARTTHREAD 3

#define ERR_OSSTOPTIMER 77

#define ERR_OSUNMAPTLB 12

#define ERR_OSVIGETCURRENTFRAMEBUFFER 37

#define ERR_OSVIGETCURRENTMODE 36

#define ERR_OSVIGETNEXTFRAMEBUFFER 38

#define ERR_OSVISETEVENT 46

#define ERR_OSVISETMODE 45

#define ERR_OSVISETSPECIAL_VALUE 43

#define ERR_OSVISETSPECIAL_VIMGR 44

#define ERR_OSVISETXSCALE_VALUE 39

#define ERR_OSVISETXSCALE_VIMGR 40

#define ERR_OSVISETYSCALE_VALUE 41

#define ERR_OSVISETYSCALE_VIMGR 42

#define ERR_OSVISWAPBUFFER_ADDR 47

#define ERR_OSVISWAPBUFFER_VIMGR 48

#define ERR_OSWRITEHOST_ADDR 72

#define ERR_OSWRITEHOST_SIZE 73

#define OS_ERROR_MAGIC 1801548921

#define OS_LOG_MAGIC 544434803

#define OS_LOG_MAX_ARGS 16

#define OS_LOG_VERSION 1

#define FALSE 0

#define NULL 0

#define TRUE 1

#define UP_CELLNL 32

#define UP_MAXCELLS 50

#define UP_MAXPVERTS 16

#define UP_OBNL 32

typedef struct AssetLoad_Common AssetLoad_Common, *PAssetLoad_Common;

typedef enum AssetType {
    DLL=5
} AssetType;

struct AssetLoad_Common {
    undefined field_0x0;
    enum AssetType type;
};

typedef struct AssetLoad_DLL AssetLoad_DLL, *PAssetLoad_DLL;

struct AssetLoad_DLL {
    struct AssetLoad_Common field_0x0;
    undefined field_0x2;
    undefined field_0x3;
    u32 id;
    struct DLL * loadTo;
    u32 param;
};

typedef enum DataFileEnum32 {
    DLLS_BIN=70,
    ANIM_BIN=52,
    TEX0_BIN=39,
    ANIM_TAB=51,
    TEX1_BIN=36
} DataFileEnum32;

typedef enum DebugError {
    BadCommand=4294967295,
    BadState=4294967292,
    PacketTooLong=4294967288,
    BadParam=4294967294,
    BadAddress=4294967291,
    Success=0,
    RunningThread=1
} DebugError;

typedef struct FuncCallRecord FuncCallRecord, *PFuncCallRecord;

struct FuncCallRecord {
    undefined4 field_0x0;
    undefined4 field_0x4;
    char * file;
    int line;
    undefined4 field_0x10;
};

typedef struct Internal_Header Internal_Header, *PInternal_Header;

struct Internal_Header {
    dword Magic;
    dword Clock Rate;
    dword Load Address;
    dword Release Offset;
    dword CRC1;
    dword CRC2;
    dword Unknown 5;
    dword Unknown 6;
    char Game Title[20];
    dword Zeroed;
    word Zeroed;
    byte Zeroed;
    byte Media Type;
    char Game Code[2];
    byte Region;
    byte Mask ROM Version;
};

typedef struct OSContPad OSContPad, *POSContPad;

struct OSContPad {
    u16 buttons;
    s8 stickX;
    s8 stickY;
    u8 error;
    undefined field_0x5;
};

typedef struct OSThreadEvent OSThreadEvent, *POSThreadEvent;

struct OSThreadEvent {
    struct OSThreadQueue * queue;
    void * * msg;
};

typedef struct PadState PadState, *PPadState;

struct PadState {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    undefined field_0x8;
    undefined field_0x9;
    undefined field_0xa;
    undefined field_0xb;
    undefined field_0xc;
    undefined field_0xd;
    undefined field_0xe;
    undefined field_0xf;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
    struct OSContPad curButtons[4];
    struct OSContPad prevButtons[4];
    u16 newButtons[4];
};

typedef enum RomOffset {
    FileBase=674464,
    FileTable=674160
} RomOffset;

typedef struct SP_DMEM_Header SP_DMEM_Header, *PSP_DMEM_Header;

struct SP_DMEM_Header {
    undefined4 _SP_DMEM_header;
    undefined4 field_0x4;
    undefined4 field_0x8;
    undefined4 field_0xc;
};

typedef unsigned short    wchar16;
