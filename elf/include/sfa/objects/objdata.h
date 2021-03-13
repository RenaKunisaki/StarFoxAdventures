typedef enum { //type:u8
	ObjectFileStructFlags76_Animated = 0x1,
} ObjectFileStructFlags76;

typedef enum { //type:u32
    ObjFileStructFlags44_HaveModels               =      0x1,
	ObjFileStructFlags44_DifferentLightColor      =     0x10,
    ObjFileStructFlags44_ModelRelated             =     0x20,
	ObjFileStructFlags44_HasChildren              =     0x40,
    ObjFileStructFlags44_EnableCulling            =    0x400,
    ObjFileStructFlags44_UseDifferentModelLoading =    0x800,
    ObjFileStructFlags44_SeqMtxRelated            =   0x8000,
    ObjFileStructFlags44_LockAnimsAndControls     =  0x40000,
	ObjFileStructFlags44_DifferentCulling         =  0x80000,
    ObjFileStructFlags44_KeepHitboxWhenInvisible  = 0x200000,
    ObjFileStructFlags44_HasEvent                 = 0x400000,
	ObjFileStructFlags44_DidLoadModels            = 0x800000,
} ObjFileStructFlags44;

typedef enum { //type:u8
    ObjFileStructFlags5F_CrazyTranslucentEffect        =  0x1,
	ObjFileStructFlags5F_ShadowUsesNoTexture           =  0x2,
    ObjFileStructFlags5F_ShadowUsesDepthTest           =  0x4,
    ObjFileStructFlags5F_DontFollowParentRotation      =  0x8,
	ObjFileStructFlags5F_NoShadow_forceDepthTestWith01 = 0x10,
	ObjFileStructFlags5F_Visible                       = 0x20,
    ObjFileStructFlags5F_DifferentTexturesSuperDark    = 0x80,
} ObjFileStructFlags5F;

typedef enum { //type:u16
    ObjFileStruct_ShadowType_NoShadow        = 0x0,
    ObjFileStruct_ShadowType_BigBoxShadow    = 0x1,
	ObjFileStruct_ShadowType_GeomShadow      = 0x2,
    ObjFileStruct_ShadowType_Crash           = 0x3,
	ObjFileStruct_ShadowType_BlueGlowingRect = 0x4,
} ObjFileStruct_ShadowType;

typedef struct PACKED AttachPoint {
	vec3f pos;
	vec3s rot;
	byte  bone;
	byte  unk13;
	byte  unk14;
	u8    unk15;
	u8    unk16;
	u8    unk17;
} AttachPoint;

typedef struct PACKED ObjectFileStruct {
	float                    unk00;              //0x00
	float                    scale;              //0x04
	u32                     *pModelList;         //0x08
	u32                     *textures;           //0x0C
	void                    *unk10;              //0x10
	void                    *unk14;              //0x14
	ObjSeqCmd               *seqCmds;            //0x18
	u16                     *pSeq;               //0x1C
	void                    *pEvent;             //0x20
	void                    *pHits;              //0x24
	void                    *pWeaponDa;          //0x28
	AttachPoint             *pAttachPoints;      //0x2C
	short                   *pModLines;          //0x30
	void                    *pIntersectPoints;   //0x34
	void                    *nextIntersectPoint; //0x38
	void                    *nextIntersectLine;  //0x3C
	void                    *aButtonInteraction; //0x40 AButtonInteraction*
	u32                      flags;              //0x44 ObjFileStructFlags44
	u16                      shadowType;         //0x48 ObjFileStruct_ShadowType
	s16                      shadowTexture;      //0x4A
	u16                      hitbox_flags60;     //0x4E HitboxFlags60
	s16                      dll_id;             //0x50 DLL_ID
	u16                      clsId;              //0x52 ObjCatId
	byte                     nModels;            //0x55
	byte                     numPlayerObjs;      //0x56
	u8                       unk57;              //0x57
	u8                       nAttachPoints;      //0x58
	byte                     nTextures;          //0x59
	u8                       numVecs;            //0x5A
	byte                     modLinesSize;       //0x5C
	s8                       modLinesIdx;        //0x5D
	u8                       numSeqs;            //0x5E
	u8                       flags_0x5f;         //0x5F ObjFileStructFlags5F
	byte                     hitbox_fieldB0;     //0x60
	byte                     hasHitbox;          //0x61
	byte                     hitboxSizeXY;       //0x62
	byte                     hitbox_field6A;     //0x63
	undefined1               hitbox_field6B;     //0x64
	u8                       hitbox_flags62;     //0x65 HitboxFlags62
	byte                     unk66;              //0x66
	byte                     hitbox_fieldB5;     //0x67
	short                    hitboxSizeX1;       //0x68
	word                     hitboxSizeY1;       //0x6A
	short                    hitboxSizeZ1;       //0x6C
	short                    hitboxSizeZ2;       //0x6E
	byte                     hitbox_fieldB4;     //0x70
	u8                       flags_0x71;         //0x71
	byte                     numFocusPoints;     //0x72
	byte                     cullDistance;       //0x73
	u8                       flags76;            //0x76 ObjectFileStructFlags76
	byte                     hitboxSizeZ;        //0x77
	s16                      map;                //0x78 MapDirIdx16
	s16                      unk7A;              //0x7A
	s16                      helpTexts;          //0x7C GameTextId
	undefined2               unk84;              //0x84
	undefined2               unk86;              //0x86
	float                    lagVar88;           //0x88
	byte                     nLights;            //0x8C
	byte                     lightIdx;           //0x8D
	byte                     colorIdx;           //0x8E
	byte                     unk8F;              //0x8F
	u8                       hitbox_flagsB6;     //0x90 HitboxFlags62
	char                     name[11];           //0x91
} ObjectFileStruct;
