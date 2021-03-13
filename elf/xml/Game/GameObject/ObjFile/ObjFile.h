//include/sfa/xml/Game/GameObject/ObjFile.xml
typedef enum { //type:u8
	ObjectFileStructFlags76_Animated = 0x1,
} ObjectFileStructFlags76;
//include/sfa/xml/Game/GameObject/ObjFile.xml
typedef enum { //type:u32
	ObjFileStructFlags44_EnableCulling = 0x400,
	ObjFileStructFlags44_DifferentLightColor = 0x10,
	ObjFileStructFlags44_HasChildren = 0x40,
	ObjFileStructFlags44_HasEvent = 0x400000,
	ObjFileStructFlags44_DifferentCulling = 0x80000,
	ObjFileStructFlags44_DidLoadModels = 0x800000,
	ObjFileStructFlags44_HaveModels = 0x1,
	ObjFileStructFlags44_LockAnimsAndControls = 0x40000,
	ObjFileStructFlags44_KeepHitboxWhenInvisible = 0x200000,
	ObjFileStructFlags44_SeqMtxRelated = 0x8000,
	ObjFileStructFlags44_ModelRelated = 0x20,
	ObjFileStructFlags44_UseDifferentModelLoading = 0x800,
} ObjFileStructFlags44;
//include/sfa/xml/Game/GameObject/ObjFile.xml
typedef enum { //type:u8
	ObjFileStructFlags5F_ShadowUsesNoTexture = 0x2,
	ObjFileStructFlags5F_DifferentTexturesSuperDark = 0x80,
	ObjFileStructFlags5F_NoShadow_forceDepthTestWith01 = 0x10,
	ObjFileStructFlags5F_ShadowUsesDepthTest = 0x4,
	ObjFileStructFlags5F_CrazyTranslucentEffect = 0x1,
	ObjFileStructFlags5F_Visible = 0x20,
	ObjFileStructFlags5F_DontFollowParentRotation = 0x8,
} ObjFileStructFlags5F;
//include/sfa/xml/Game/GameObject/ObjFile.xml
typedef enum { //type:u16
	ObjFileStruct_ShadowType_Crash = 0x3,
	ObjFileStruct_ShadowType_BigBoxShadow = 0x1,
	ObjFileStruct_ShadowType_NoShadow = 0x0,
	ObjFileStruct_ShadowType_GeomShadow = 0x2,
	ObjFileStruct_ShadowType_BlueGlowingRect = 0x4,
} ObjFileStruct_ShadowType;
//include/sfa/xml/Game/GameObject/ObjFile.xml
typedef union ObjectFileStruct {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_scale[0x4]; float scale; };
	struct __attribute__((packed)) { u8 _pad_pModelList[0x8]; u32* pModelList; };
	struct __attribute__((packed)) { u8 _pad_textures[0xC]; pointer textures; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; pointer unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; pointer unk14; };
	struct __attribute__((packed)) { u8 _pad_offset_0x18[0x18]; ObjSeqCmd* offset_0x18; };
	struct __attribute__((packed)) { u8 _pad_pSeq[0x1C]; u16* pSeq; };
	struct __attribute__((packed)) { u8 _pad_pEvent[0x20]; undefined* pEvent; };
	struct __attribute__((packed)) { u8 _pad_pHits[0x24]; undefined* pHits; };
	struct __attribute__((packed)) { u8 _pad_pWeaponDa[0x28]; undefined* pWeaponDa; };
	struct __attribute__((packed)) { u8 _pad_pAttachPoints[0x2C]; AttachPoint* pAttachPoints; };
	struct __attribute__((packed)) { u8 _pad_pModLines[0x30]; short* pModLines; };
	struct __attribute__((packed)) { u8 _pad_pIntersectPoints[0x34]; pointer pIntersectPoints; };
	struct __attribute__((packed)) { u8 _pad_nextIntersectPoint[0x38]; pointer nextIntersectPoint; };
	struct __attribute__((packed)) { u8 _pad_nextIntersectLine[0x3C]; pointer nextIntersectLine; };
	struct __attribute__((packed)) { u8 _pad_aButtonInteraction[0x40]; AButtonInteraction* aButtonInteraction; };
	struct __attribute__((packed)) { u8 _pad_flags[0x44]; ObjFileStructFlags44 flags; };
	struct __attribute__((packed)) { u8 _pad_shadowType[0x48]; ObjFileStruct_ShadowType shadowType; };
	struct __attribute__((packed)) { u8 _pad_shadowTexture[0x4A]; s16 shadowTexture; };
	struct __attribute__((packed)) { u8 _pad_hitbox_flags60[0x4E]; HitboxFlags60 hitbox_flags60; };
	struct __attribute__((packed)) { u8 _pad_dll_id[0x50]; DLL_ID dll_id; };
	struct __attribute__((packed)) { u8 _pad_clsId[0x52]; ObjCatId clsId; };
	struct __attribute__((packed)) { u8 _pad_nModels[0x55]; byte nModels; };
	struct __attribute__((packed)) { u8 _pad_numPlayerObjs[0x56]; byte numPlayerObjs; };
	struct __attribute__((packed)) { u8 _pad_unk57[0x57]; u8 unk57; };
	struct __attribute__((packed)) { u8 _pad_nAttachPoints[0x58]; u8 nAttachPoints; };
	struct __attribute__((packed)) { u8 _pad_nTextures[0x59]; byte nTextures; };
	struct __attribute__((packed)) { u8 _pad_numVecs[0x5A]; u8 numVecs; };
	struct __attribute__((packed)) { u8 _pad_modLinesSize[0x5C]; byte modLinesSize; };
	struct __attribute__((packed)) { u8 _pad_modLinesIdx[0x5D]; s8 modLinesIdx; };
	struct __attribute__((packed)) { u8 _pad_numSeqs[0x5E]; u8 numSeqs; };
	struct __attribute__((packed)) { u8 _pad_flags_0x5f[0x5F]; ObjFileStructFlags5F flags_0x5f; };
	struct __attribute__((packed)) { u8 _pad_hitbox_fieldB0[0x60]; byte hitbox_fieldB0; };
	struct __attribute__((packed)) { u8 _pad_hasHitbox[0x61]; byte hasHitbox; };
	struct __attribute__((packed)) { u8 _pad_hitboxSizeXY[0x62]; byte hitboxSizeXY; };
	struct __attribute__((packed)) { u8 _pad_hitbox_field6A[0x63]; byte hitbox_field6A; };
	struct __attribute__((packed)) { u8 _pad_hitbox_field6B[0x64]; undefined1 hitbox_field6B; };
	struct __attribute__((packed)) { u8 _pad_hitbox_flags62[0x65]; HitboxFlags62 hitbox_flags62; };
	struct __attribute__((packed)) { u8 _pad_unk66[0x66]; byte unk66; };
	struct __attribute__((packed)) { u8 _pad_hitbox_fieldB5[0x67]; byte hitbox_fieldB5; };
	struct __attribute__((packed)) { u8 _pad_hitboxSizeX1[0x68]; short hitboxSizeX1; };
	struct __attribute__((packed)) { u8 _pad_hitboxSizeY1[0x6A]; word hitboxSizeY1; };
	struct __attribute__((packed)) { u8 _pad_hitboxSizeZ1[0x6C]; short hitboxSizeZ1; };
	struct __attribute__((packed)) { u8 _pad_hitboxSizeZ2[0x6E]; short hitboxSizeZ2; };
	struct __attribute__((packed)) { u8 _pad_hitbox_fieldB4[0x70]; byte hitbox_fieldB4; };
	struct __attribute__((packed)) { u8 _pad_flags_0x71[0x71]; u8 flags_0x71; };
	struct __attribute__((packed)) { u8 _pad_numFocusPoints[0x72]; byte numFocusPoints; };
	struct __attribute__((packed)) { u8 _pad_cullDistance[0x73]; byte cullDistance; };
	struct __attribute__((packed)) { u8 _pad_flags76[0x76]; ObjectFileStructFlags76 flags76; };
	struct __attribute__((packed)) { u8 _pad_hitboxSizeZ[0x77]; byte hitboxSizeZ; };
	struct __attribute__((packed)) { u8 _pad_map[0x78]; MapDirIdx16 map; };
	struct __attribute__((packed)) { u8 _pad_unk7A[0x7A]; s16 unk7A; };
	struct __attribute__((packed)) { u8 _pad_helpTexts[0x7C]; GameTextId helpTexts; };
	struct __attribute__((packed)) { u8 _pad_unk84[0x84]; undefined2 unk84; };
	struct __attribute__((packed)) { u8 _pad_unk86[0x86]; undefined2 unk86; };
	struct __attribute__((packed)) { u8 _pad_lagVar88[0x88]; float lagVar88; };
	struct __attribute__((packed)) { u8 _pad_nLights[0x8C]; byte nLights; };
	struct __attribute__((packed)) { u8 _pad_lightIdx[0x8D]; byte lightIdx; };
	struct __attribute__((packed)) { u8 _pad_colorIdx[0x8E]; byte colorIdx; };
	struct __attribute__((packed)) { u8 _pad_unk8F[0x8F]; byte unk8F; };
	struct __attribute__((packed)) { u8 _pad_hitbox_flagsB6[0x90]; HitboxFlags62 hitbox_flagsB6; };
	struct __attribute__((packed)) { u8 _pad_name[0x91]; char name; };
} ObjectFileStruct;
