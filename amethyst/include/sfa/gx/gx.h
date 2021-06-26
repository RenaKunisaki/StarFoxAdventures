typedef struct {
    union {
        struct { u8 r; u8 g; u8 b; u8 a; };
        u32 value;
    };
} Color4b;

typedef enum { //type:u32
    VcdHiMask_TEX0 = 0x3,
    VcdHiMask_TEX1 = 0xC,
    VcdHiMask_TEX2 = 0x30,
    VcdHiMask_TEX3 = 0xC0,
    VcdHiMask_TEX4 = 0x300,
    VcdHiMask_TEX5 = 0xC00,
	VcdHiMask_TEX6 = 0x3000,
    VcdHiMask_TEX7 = 0xC000,
} VcdHiMask;

typedef enum { //type:u32
    VcdLoMask_PMIDX  = 0x1,
    VcdLoMask_T0MIDX = 0x2,
	VcdLoMask_T1MIDX = 0x4,
    VcdLoMask_T2MIDX = 0x8,
	VcdLoMask_T3MIDX = 0x10,
    VcdLoMask_T4MIDX = 0x20,
    VcdLoMask_T5MIDX = 0x40,
	VcdLoMask_T6MIDX = 0x80,
    VcdLoMask_T7MIDX = 0x100,
    VcdLoMask_POS0   = 0x200,
    VcdLoMask_POS1   = 0x400,
    VcdLoMask_POS    = 0x600,
    VcdLoMask_NRM0   = 0x800,
    VcdLoMask_NRM1   = 0x1000,
	VcdLoMask_NRM    = 0x1800,
    VcdLoMask_COL00  = 0x2000,
    VcdLoMask_COL01  = 0x4000,
	VcdLoMask_COL0   = 0x6000,
    VcdLoMask_COL10  = 0x8000,
	VcdLoMask_COL11  = 0x10000,
    VcdLoMask_COL1   = 0x18000,
} VcdLoMask;

typedef enum {
    RenderFlag_IsNotSubmap              = 0x1,
    RenderFlag_EnableMapUpdate          = 0x2,
    RenderFlag_Widescreen               = 0x8,
    RenderFlag_Cloudy                   = 0x10,
    RenderFlag_DrawLights               = 0x40,
    RenderFlag_DrawShadows              = 0x80,
    RenderFlag_PendingMapLoad           = 0x1000,
    RenderFlag_ChangeMapLayer           = 0x4000,
    RenderFlag_ChangeDrawDistance_10000 = 0x10000,
    RenderFlag_Overcast                 = 0x40000,
} RenderFlags;

typedef struct {
    void *cmdBuf;   //commands to execute
    void *unk04;    //always NULL?
    void *frameBuf; //frame buffer to render into
} GxPendingFrame;

typedef struct {
    short           nPending;   //number of pending frames
    short           queueSize;  //max pending frames
    short           entrySize;  //sizeof(GxPendingFrame)
    u8              unk06;
    u8              unk07;
    short           next;       //idx of next queue entry to write
    u8              unk0A;
    u8              unk0B;
    GxPendingFrame *frame;
} GxFrameQueue;

extern GxFrameQueue gxFrameQueue;