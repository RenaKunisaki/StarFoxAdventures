//include/sfa/xml/Animation/Model.xml
typedef enum { //type:u32
	astruct39_flags3C_WeirdSkinTexture = 0x100000,
	astruct39_flags3C_Translucent = 0x40000000,
} astruct39_flags3C;
//include/sfa/xml/Animation/Model.xml
typedef enum { //type:u16
	ModelFlags_FIELD_18_NOT_ZERO = 0x40,
	ModelFlags_NO_EXTRA_AMAP = 0x2,
} ModelFlags;
//include/sfa/xml/Animation/Model.xml
typedef enum { //type:u16
	ModelFlags18_UseOtherHitboxes = 0x4,
	ModelFlags18_ShadersLoaded = 0x40,
	ModelFlags18_MtxsLoaded = 0x8,
	ModelFlags18_UseOtherVtxs = 0x2,
	ModelFlags18_UseOtherMtxs = 0x1,
} ModelFlags18;
//include/sfa/xml/Animation/Model.xml
typedef enum { //type:u32
	ModelFlags_loadCharacter_HasShadowType3 = 0x8000,
	ModelFlags_loadCharacter_HasEvent = 0x40,
	ModelFlags_loadCharacter_OnlyLoadOneModel = 0x400,
	ModelFlags_loadCharacter_HasSomething0x800Bytes = 0x100,
	ModelFlags_loadCharacter_objFileHasModels = 0x200,
	ModelFlags_loadCharacter_objFileFlags20isZero = 0x1,
	ModelFlags_loadCharacter_ModelIdMask = 0x7800,
	ModelFlags_loadCharacter_HasShadow = 0x2,
} ModelFlags_loadCharacter;
//include/sfa/xml/Animation/Model.xml
typedef enum { //type:u16
	ModelId_AlsoFox = 0x3,
	ModelId_SharpClaw = 0x4E9,
	ModelId_Krystal = 0x4E8,
	ModelId_Fox = 0x1,
} ModelId;
typedef undefined (*modelField38)(ObjInstance* param1, Model* param2, undefined1 param3);
//include/sfa/xml/Animation/Model.xml
typedef union Model {
	ModelFileHeader* header;
	struct __attribute__((packed)) { u8 _pad_cacheModNo[0x4]; short cacheModNo; };
	struct __attribute__((packed)) { u8 _pad_mtxs[0xC]; Mtx44* mtxs; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; float* unk14; };
	struct __attribute__((packed)) { u8 _pad_flags[0x18]; ModelFlags18 flags; };
	struct __attribute__((packed)) { u8 _pad_vtxs[0x1C]; vec3s* vtxs; };
	struct __attribute__((packed)) { u8 _pad_normals[0x24]; vec3s* normals; };
	struct __attribute__((packed)) { u8 _pad_animations[0x28]; ModelAnimStruct2* animations; };
	struct __attribute__((packed)) { u8 _pad_shaderDefs[0x34]; ShaderDef* shaderDefs; };
	struct __attribute__((packed)) { u8 _pad_texFuncPtr[0x38]; modelField38* texFuncPtr; };
	struct __attribute__((packed)) { u8 _pad_fptr_onSetupShader[0x3C]; pointer fptr_onSetupShader; };
	struct __attribute__((packed)) { u8 _pad_pAnimBuf[0x40]; dword pAnimBuf; };
	struct __attribute__((packed)) { u8 _pad_unk44[0x44]; pointer unk44; };
	struct __attribute__((packed)) { u8 _pad_hitSpherePositions[0x48]; HitSpherePos* hitSpherePositions; };
	struct __attribute__((packed)) { u8 _pad_curHitSpherePos[0x50]; HitSpherePos* curHitSpherePos; };
	struct __attribute__((packed)) { u8 _pad_unk54[0x54]; pointer unk54; };
	struct __attribute__((packed)) { u8 _pad_unk58[0x58]; pointer unk58; };
	struct __attribute__((packed)) { u8 _pad_mtxBufs[0x5C]; Mtx44* mtxBufs; };
	struct __attribute__((packed)) { u8 _pad_areBoneTransformsApplied[0x60]; byte areBoneTransformsApplied; };
} Model;
//include/sfa/xml/Animation/Model.xml
typedef union ModelAnimStruct {
	int vtxSize;
	struct __attribute__((packed)) { u8 _pad_hitboxSize[0x4]; int hitboxSize; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; int unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; int unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; int unk14; };
	struct __attribute__((packed)) { u8 _pad_mtxSize[0x18]; int mtxSize; };
} ModelAnimStruct;
//include/sfa/xml/Animation/Model.xml
typedef union ModelAnimStruct2 {
	ModelAnimStruct56 unk00;
	struct __attribute__((packed)) { u8 _pad_unk30[0x30]; undefined4 unk30; };
	struct __attribute__((packed)) { u8 _pad_unk34[0x34]; undefined4 unk34; };
	struct __attribute__((packed)) { u8 _pad_unk38[0x38]; undefined4 unk38; };
	struct __attribute__((packed)) { u8 _pad_unk3C[0x3C]; undefined4 unk3C; };
	struct __attribute__((packed)) { u8 _pad_unk40[0x40]; undefined4 unk40; };
	struct __attribute__((packed)) { u8 _pad_animIdx[0x44]; u16 animIdx; };
	struct __attribute__((packed)) { u8 _pad_unk46[0x46]; u16 unk46; };
	struct __attribute__((packed)) { u8 _pad_unk48[0x48]; u16 unk48; };
	struct __attribute__((packed)) { u8 _pad_unk4A[0x4A]; u16 unk4A; };
	struct __attribute__((packed)) { u8 _pad_unk4C[0x4C]; undefined2 unk4C; };
	struct __attribute__((packed)) { u8 _pad_unk50[0x50]; undefined2 unk50; };
	struct __attribute__((packed)) { u8 _pad_unk58[0x58]; u16 unk58; };
	struct __attribute__((packed)) { u8 _pad_unk5A[0x5A]; u16 unk5A; };
	struct __attribute__((packed)) { u8 _pad_unk5C[0x5C]; u16 unk5C; };
	struct __attribute__((packed)) { u8 _pad_unk5E[0x5E]; short unk5E; };
	struct __attribute__((packed)) { u8 _pad_bLoop[0x60]; byte bLoop; };
	struct __attribute__((packed)) { u8 _pad_unk61[0x61]; byte unk61; };
	struct __attribute__((packed)) { u8 _pad_negativeAnimIdx[0x62]; byte negativeAnimIdx; };
	struct __attribute__((packed)) { u8 _pad_flags[0x63]; byte flags; };
	struct __attribute__((packed)) { u8 _pad_unk64[0x64]; undefined2 unk64; };
} ModelAnimStruct2;
//include/sfa/xml/Animation/Model.xml
typedef union ModelAnimStruct28 {
	float x;
	struct __attribute__((packed)) { u8 _pad_y[0x4]; float y; };
	struct __attribute__((packed)) { u8 _pad_z[0x8]; float z; };
	struct __attribute__((packed)) { u8 _pad_start[0xC]; s8 start; };
	struct __attribute__((packed)) { u8 _pad_end[0xD]; s8 end; };
	struct __attribute__((packed)) { u8 _pad_flags[0xE]; AnimStruct28Flags flags; };
	struct __attribute__((packed)) { u8 _pad_unk0F[0xF]; u8 unk0F; };
} ModelAnimStruct28;
//include/sfa/xml/Animation/Model.xml
typedef union ModelList {
	ModelListEntry* pModelHeader;
	struct __attribute__((packed)) { u8 _pad_pEnd[0x4]; void* pEnd; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; void* unk08; };
	struct __attribute__((packed)) { u8 _pad_headerSize[0xC]; u8 headerSize; };
	struct __attribute__((packed)) { u8 _pad_stride[0xD]; byte stride; };
	struct __attribute__((packed)) { u8 _pad_pCurModel[0x10]; ModelListEntry* pCurModel; };
} ModelList;
//include/sfa/xml/Animation/Model.xml
typedef union ModelRenderInstrsState {
	byte* data;
	struct __attribute__((packed)) { u8 _pad_nBytes[0x4]; int nBytes; };
	struct __attribute__((packed)) { u8 _pad_nBits[0x8]; uint nBits; };
	struct __attribute__((packed)) { u8 _pad_nBits2[0xC]; int nBits2; };
	struct __attribute__((packed)) { u8 _pad_bit[0x10]; int bit; };
} ModelRenderInstrsState;
//include/sfa/xml/Animation/Model.xml
typedef union ModelRenderInstrStruct57 {
	struct __attribute__((packed)) { u8 _pad_unk06[0x6]; short unk06; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; short unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0A[0xA]; short unk0A; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; short unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk0E[0xE]; short unk0E; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; short unk10; };
	struct __attribute__((packed)) { u8 _pad_curBit[0x14]; ushort curBit; };
} ModelRenderInstrStruct57;
//include/sfa/xml/Animation/Model.xml
typedef union ModelStruct55 {
	void* unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; int unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; float unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; float unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; float unk14; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; byte unk18; };
	struct __attribute__((packed)) { u8 _pad_unk19[0x19]; byte unk19; };
	struct __attribute__((packed)) { u8 _pad_unk1A[0x1A]; byte unk1A; };
	struct __attribute__((packed)) { u8 _pad_color[0x78]; Color4b color; };
	struct __attribute__((packed)) { u8 _pad_color2[0x80]; Color4b color2; };
	struct __attribute__((packed)) { u8 _pad_unk88[0x88]; Color4b unk88; };
	struct __attribute__((packed)) { u8 _pad_vectors[0x90]; vec3f vectors; };
	struct __attribute__((packed)) { u8 _pad_vectors2[0x9C]; vec3f vectors2; };
} ModelStruct55;
//include/sfa/xml/Animation/Model.xml
typedef union ModelVtxGroup {
	u8 bone0;
	struct __attribute__((packed)) { u8 _pad_bone1[0x1]; u8 bone1; };
	struct __attribute__((packed)) { u8 _pad_weight[0x2]; u8 weight; };
	struct __attribute__((packed)) { u8 _pad_unk03[0x3]; u8 unk03; };
} ModelVtxGroup;
//include/sfa/xml/Animation/Model.xml
typedef union ObjLight {
	ObjInstance* obj;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; vec3f unk04; };
	struct __attribute__((packed)) { u8 _pad_lightVec[0x10]; vec3f lightVec; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; vec3f unk1C; };
	struct __attribute__((packed)) { u8 _pad_vIn[0x28]; vec3f vIn; };
	struct __attribute__((packed)) { u8 _pad_vOut[0x34]; vec3f vOut; };
	struct __attribute__((packed)) { u8 _pad_vCamera[0x40]; vec3f vCamera; };
	struct __attribute__((packed)) { u8 _pad_unk4D[0x4D]; byte unk4D; };
	struct __attribute__((packed)) { u8 _pad_unk50[0x50]; uint unk50; };
	struct __attribute__((packed)) { u8 _pad_unk54[0x54]; undefined* unk54; };
	struct __attribute__((packed)) { u8 _pad_unk58[0x58]; undefined4 unk58; };
	struct __attribute__((packed)) { u8 _pad_unk5C[0x5C]; undefined4 unk5C; };
	struct __attribute__((packed)) { u8 _pad_unk60[0x60]; pointer unk60; };
	struct __attribute__((packed)) { u8 _pad_lightFlags_0x64[0x64]; byte lightFlags_0x64; };
	struct __attribute__((packed)) { u8 _pad_gxLight[0x68]; GXLightObj gxLight; };
	struct __attribute__((packed)) { u8 _pad_lightColorA8[0xA8]; Color4b lightColorA8; };
	struct __attribute__((packed)) { u8 _pad_unkAC[0xAC]; Color4b unkAC; };
	struct __attribute__((packed)) { u8 _pad_unkB0[0xB0]; Color4b unkB0; };
	struct __attribute__((packed)) { u8 _pad_unkB4[0xB4]; float unkB4; };
	struct __attribute__((packed)) { u8 _pad_unkB8[0xB8]; undefined4 unkB8; };
	struct __attribute__((packed)) { u8 _pad_unkBC[0xBC]; byte unkBC; };
	struct __attribute__((packed)) { u8 _pad_light_c0[0xC0]; GXLightObj light_c0; };
	struct __attribute__((packed)) { u8 _pad_lightColor[0x100]; Color4b lightColor; };
	struct __attribute__((packed)) { u8 _pad_unk104[0x104]; Color4b unk104; };
	struct __attribute__((packed)) { u8 _pad_unk108[0x108]; Color4b unk108; };
	struct __attribute__((packed)) { u8 _pad_unk10C[0x10C]; float unk10C; };
	struct __attribute__((packed)) { u8 _pad_unk110[0x110]; float unk110; };
	struct __attribute__((packed)) { u8 _pad_unk114[0x114]; byte unk114; };
	struct __attribute__((packed)) { u8 _pad_distAttenA0[0x124]; float distAttenA0; };
	struct __attribute__((packed)) { u8 _pad_distAttenA1[0x128]; float distAttenA1; };
	struct __attribute__((packed)) { u8 _pad_distAttenA2[0x12C]; float distAttenA2; };
	struct __attribute__((packed)) { u8 _pad_colorVal_0x130[0x130]; float colorVal_0x130; };
	struct __attribute__((packed)) { u8 _pad_lightAmount[0x134]; float lightAmount; };
	struct __attribute__((packed)) { u8 _pad_unk138[0x138]; float unk138; };
	struct __attribute__((packed)) { u8 _pad_unk13C[0x13C]; float unk13C; };
	struct __attribute__((packed)) { u8 _pad_unk140[0x140]; float unk140; };
	struct __attribute__((packed)) { u8 _pad_unk144[0x144]; float unk144; };
	struct __attribute__((packed)) { u8 _pad_unk150[0x150]; float unk150; };
	struct __attribute__((packed)) { u8 _pad_unk154[0x154]; float unk154; };
	struct __attribute__((packed)) { u8 _pad_unk158[0x158]; float unk158; };
	struct __attribute__((packed)) { u8 _pad_unk15C[0x15C]; float unk15C; };
	struct __attribute__((packed)) { u8 _pad_unk160[0x160]; float unk160; };
	struct __attribute__((packed)) { u8 _pad_unk164[0x164]; float unk164; };
	struct __attribute__((packed)) { u8 _pad_unk168[0x168]; dword unk168; };
	struct __attribute__((packed)) { u8 _pad_unk16C[0x16C]; dword unk16C; };
	struct __attribute__((packed)) { u8 _pad_unk170[0x170]; Mtx43 unk170; };
	struct __attribute__((packed)) { u8 _pad_unk1F0[0x1F0]; Mtx43 unk1F0; };
	struct __attribute__((packed)) { u8 _pad_unk2D8[0x2D8]; int unk2D8; };
	struct __attribute__((packed)) { u8 _pad_unk2DC[0x2DC]; float unk2DC; };
	struct __attribute__((packed)) { u8 _pad_unk2E0[0x2E0]; float unk2E0; };
	struct __attribute__((packed)) { u8 _pad_unk2E4[0x2E4]; float unk2E4; };
	struct __attribute__((packed)) { u8 _pad_texture[0x2E8]; Texture* texture; };
	struct __attribute__((packed)) { u8 _pad_unk2F8[0x2F8]; byte unk2F8; };
	struct __attribute__((packed)) { u8 _pad_colorVal_0x2fc[0x2FC]; byte colorVal_0x2fc; };
} ObjLight;
