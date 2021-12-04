#define MAX_ENVFX_ID 1024
#define MAX_USED_ENVFX_ID 0x249 //after this is all zeros

typedef enum {
    ENVFX_TYPE_RAIN = 0,
    ENVFX_TYPE_FOG = 3,
    ENVFX_TYPE_SNOW = 4, //also rain?
    ENVFX_TYPE_COLOR = 5, //XXX what is this?
    ENVFX_TYPE_CLOUDS = 6,
} EnvFxType;
//not sure if maybe the difference between 0 and 4 is whether they follow you?

typedef struct PACKED {
/*0x00*/ float unk00;
/*0x04*/ float unk04;
/*0x08*/ float skyMoveSpeed;
/*0x0c*/ u8	red[8];	//1 byte per 3 hours
/*0x14*/ u8	green[8];
/*0x1c*/ u8	blue[8];
/*0x24*/ s16 unk24;
/*0x26*/ u16 cloudIdx;
/*0x28*/ u16 intensity;
/*0x2a*/ u16 fadeTimer; //maybe also cloud X speed?
/*0x2c*/ u16 cloudTimer2C;
/*0x2e*/ u16 visibility; //inverse fog intensity
/*0x30*/ u16 unk30;
/*0x32*/ u16 unk32;
/*0x34*/ u16 unk34;
/*0x36*/ u8 unk36;
/*0x37*/ u8 unk37;
/*0x38*/ u8 unk38;
/*0x39*/ u8 unk39;
/*0x3a*/ u8 unk3a;
/*0x3b*/ u8 unk3b;
/*0x3c*/ u8 unk3c;
/*0x3d*/ u8 unk3d;
/*0x3e*/ u16 unk3e;
/*0x40*/ u16 unk40;
/*0x42*/ u16 unk42;
/*0x44*/ u16 unk44;
/*0x46*/ u8 unk46;
/*0x47*/ u8 unk47;
/*0x48*/ u8 unk48;
/*0x49*/ u8 unk49;
/*0x4a*/ u8 unk4a;
/*0x4b*/ u8 unk4b;
/*0x4c*/ u8 unk4c;
/*0x4d*/ u8 unk4d;
/*0x4e*/ u8 unk4e;
/*0x4f*/ u8 unk4f;
/*0x50*/ u8 unk50;
/*0x51*/ u8 unk51;
/*0x52*/ u8 unk52;
/*0x53*/ u8 unk53;
/*0x54*/ s16 unk54; //type of something
/*0x56*/ u16 unk56;
/*0x58*/ u8 cloudFlags; //1:rotate, 2:enable, 20:kill
/*0x59*/ u8 flags59;
/*0x5a*/ u8 skyObjIdx3; //also scroll speed for snow
/*0x5b*/ u8 skyObjIdx2;
/*0x5c*/ u8 type; //EnvFxType
/*0x5d*/ u8 skyObjIdx1;
/*0x5e*/ u8 unk5e;
/*0x5f*/ u8 unk5f;
} EnvFxActEntry;
CASSERT(sizeof(EnvFxActEntry) == 0x60, sizeof_EnvFxActEntry);
