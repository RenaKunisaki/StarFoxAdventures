typedef enum { //type:u16
    ModelFlags18_UseOtherMtxs     = 0x1,
    ModelFlags18_UseOtherVtxs     = 0x2,
	ModelFlags18_UseOtherHitboxes = 0x4,
    ModelFlags18_MtxsLoaded       = 0x8,
	ModelFlags18_ShadersLoaded    = 0x40,
} ModelFlags18;

typedef enum { //type:u16
    ModelDataFlags2_NoAnimations       =    0x2,
    ModelDataFlags2_CopyVtxsOnLoad     =   0x10,
    ModelDataFlags2_BoneRelated        =   0x20,
    ModelDataFlags2_UseLocalModAnimTab =   0x40,
    ModelDataFlags2_FogRelated         =  0x100,
    ModelDataFlags2_NoDepthTest        =  0x400,
	ModelDataFlags2_ModelField14Valid  = 0x1000,
	ModelDataFlags2_AlphaZUpdateEnable = 0x2000,
    ModelDataFlags2_AltRenderInstrs    = 0x8000,
} ModelDataFlags2;

typedef enum { //type:u8
	ModelDataFlags24_VeryBright = 0x2,
	ModelDataFlags24_Use9NormalsInsteadOf3 = 0x8,
} ModelDataFlags24;

typedef enum { //type:u16
	ModelHeaderFlagsE2_UsePlayerColorOverride =   0x2,
    ModelHeaderFlagsE2_MatFlag4               =   0x4,
    ModelHeaderFlagsE2_MatFlag8               =   0x8,
	ModelHeaderFlagsE2_MatFlag200             = 0x200,
	ModelHeaderFlagsE2_MatFlag400             = 0x400,
	ModelHeaderFlagsE2_MatFlag800             = 0x800,
} ModelHeaderFlagsE2;

typedef enum { //type:u32
    ModelFlags_loadCharacter_objFileFlags20isZero   = 0x1,
    ModelFlags_loadCharacter_HasShadow              = 0x2,
    ModelFlags_loadCharacter_HasEvent               = 0x40,
    ModelFlags_loadCharacter_HasSomething0x800Bytes = 0x100,
    ModelFlags_loadCharacter_objFileHasModels       = 0x200, //debug model?
	ModelFlags_loadCharacter_OnlyLoadOneModel       = 0x400,
	ModelFlags_loadCharacter_ModelIdMask            = 0x7800,
    ModelFlags_loadCharacter_HasShadowType3         = 0x8000,
} ModelFlags_loadCharacter;

typedef struct PACKED Bone {
	s8    parent;          //0x00
	byte  idx;             //0x01
    u16   unk02;           //0x02
	vec3f translation;     //0x04
	vec3f bindTranslation; //0x10
} Bone;

typedef struct PACKED ModelFileHeader {
	u8                  refCount;               //0x00
    u8                  unk01;                  //0x01
	u16                 flags;                  //0x02 ModelDataFlags2
	u32                 modelId;                //0x04 ModelId
	dword               unk08;                  //0x08
	u32                 fileSize;               //0x0C
    u32                 unk10;                  //0x10
	dword               unk14;                  //0x14
	float              *flags18;                //0x18
	u32                 extraAmapSize;          //0x1C
	uint               *textures;               //0x20
	u8                  flags_0x24;             //0x24 ModelDataFlags24
	byte                unk25;                  //0x25
    u8                  unk26;                  //0x26
    u8                  unk27;                  //0x27
	vec3s              *vtxs;                   //0x28
	vec3s              *normals;                //0x2C
	u16                *colors;                 //0x30
	vec2s              *texCoords;              //0x34
	void               *shaders;                //0x38 Shader*
	Bone               *bones;                  //0x3C
	Quaternion         *boneQuats;              //0x40
	u32                 unk44;                  //0x44
	u32                 unk48;                  //0x48
	u32                 unk4C;                  //0x4C
	u32                 unk50;                  //0x50
	void               *vtxGroups;              //0x54 ModelVtxGroup*
	HitSphere          *hitSpheres;             //0x58
    u32                 unk5C;                  //0x5C
    u32                 unk60;                  //0x60
	u32                *pAltIndBuf;             //0x64
	void               *amapBin;                //0x68
	short              *animIds;                //0x6C
	ushort              animIdxs[8];            //0x70
	u32                 amapTab;                //0x80
	ushort              unk84;                  //0x84
    ushort              unk86;                  //0x86
	void               *posFineSkinningConfig;  //0x88 FineSkinningConfig*
    u32                 unk8C;                  //0x8C
    u32                 unk90;                  //0x90
    u32                 unk94;                  //0x94
    u32                 unk98;                  //0x98
    u32                 unk9C;                  //0x9C
    u32                 unkA0;                  //0xA0
	void               *posFineSkinningPieces;  //0xA4 FineSkinningPiece*
	void               *posFineSkinningWeights; //0xA8
	void               *nrmFineSkinningConfig;  //0xAC FineSkinningConfig*
    u32                 unkB0;                  //0xB0
    u32                 unkB4;                  //0xB4
	dword               unkB8;                  //0xB8
    u32                 unkBC;                  //0xBC
    u32                 unkC0;                  //0xC0
    u32                 unkC4;                  //0xC4
	void               *bCopyNormalsOnLoad;     //0xC8 XXX probably wrong
	void               *unkCC;                  //0xCC
	DisplayListPtr     *dlists;                 //0xD0
	byte               *renderInstrs;           //0xD4
	ushort              nRenderInstrs;          //0xD8
    ushort              unkDA;                  //0xDA
	short             **animations;             //0xDC
	short               cullDistance;           //0xE0
	u16                 flagsE2;                //0xE2 ModelHeaderFlagsE2
	short               nVtxs;                  //0xE4
	short               nNormals;               //0xE6
	short               nColors;                //0xE8
	short               nTexCoords;             //0xEA
	short               nAnimations;            //0xEC
    short               unkEE;                  //0xEE
    short               unkF0;                  //0xF0
	u8                  nTextures;              //0xF2
	u8                  nBones;                 //0xF3
	u8                  nVtxGroups;             //0xF4
	u8                  nDlists;                //0xF5
	u8                  unkF6;                  //0xF6
	u8                  nHitSpheres;            //0xF7
	u8                  nShaders;               //0xF8
	u8                  nPtrsDC;                //0xF9
	u8                  nTexMtxs;               //0xFA
	u8                  unkFB;                  //0xFB
} ModelFileHeader;
CASSERT(sizeof(ModelFileHeader) == 0xFC, sizeof_ModelFileHeader);

typedef struct PACKED ModelListEntry {
	s32 modelId; //ModelId
	ModelFileHeader *data;
} ModelListEntry;

typedef struct PACKED ModelsBinEntry {
	u32        signature;   //0x00
	u32        size;        //0x04
    u32        unk08;       //0x08
    u32        unk0C;       //0x0C
    u32        unk10;       //0x10
    u32        unk14;       //0x14
	undefined4 noAmap;      //0x18
	undefined4 nAnimations; //0x1C
	undefined4 unk20;       //0x20
	ZlbHeader  zlb;         //0x24
} ModelsBinEntry;
CASSERT(sizeof(ModelsBinEntry) == 0x34, sizeof_ModelsBinEntry);

typedef struct PACKED Model {
	ModelFileHeader  *header;                   //0x00
	short             cacheModNo;               //0x04
    s16               unk06;                    //0x06
    u32               unk08;                    //0x08
	Mtx44            *mtxs;                     //0x0C
    u32               unk10;                    //0x10
	float            *unk14;                    //0x14
	u32              flags;                     //0x18 ModelFlags18
	vec3s            *vtxs;                     //0x1C
    void             *unk20;                    //0x20
	vec3s            *normals;                  //0x24
	void             *animations;               //0x28 ModelAnimStruct2*
    void             *unk2C;                    //0x2C
    void             *unk30;                    //0x30
	void             *shaderDefs;               //0x34 ShaderDef*
	void             *texFuncPtr;               //0x38 modelField38*
	void             *fptr_onSetupShader;       //0x3C
	dword             pAnimBuf;                 //0x40
	void             *unk44;                    //0x44
	HitSpherePos     *hitSpherePositions;       //0x48
    void             *unk4C;                    //0x4C
	HitSpherePos     *curHitSpherePos;          //0x50
	void             *unk54;                    //0x54
	void             *unk58;                    //0x58
	Mtx44            *mtxBufs;                  //0x5C
	byte              areBoneTransformsApplied; //0x60
} Model;
CASSERT(sizeof(Model) == 0x61, sizeof_Model);
