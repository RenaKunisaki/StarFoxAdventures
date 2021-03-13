//include/sfa/xml/Animation/Model/File.xml
typedef enum { //type:u16
	ModelDataFlags2_ModelField14Valid = 0x1000,
	ModelDataFlags2_AltRenderInstrs = 0x8000,
	ModelDataFlags2_BoneRelated = 0x20,
	ModelDataFlags2_NoAnimations = 0x2,
	ModelDataFlags2_CopyVtxsOnLoad = 0x10,
	ModelDataFlags2_AlphaZUpdateEnable = 0x2000,
	ModelDataFlags2_UseLocalModAnimTab = 0x40,
	ModelDataFlags2_NoDepthTest = 0x400,
	ModelDataFlags2_FogRelated = 0x100,
} ModelDataFlags2;
//include/sfa/xml/Animation/Model/File.xml
typedef enum { //type:u8
	ModelDataFlags24_VeryBright = 0x2,
	ModelDataFlags24_Use9NormalsInsteadOf3 = 0x8,
} ModelDataFlags24;
//include/sfa/xml/Animation/Model/File.xml
typedef enum { //type:u16
	ModelHeaderFlagsE2_UsePlayerColorOverride = 0x2,
	ModelHeaderFlagsE2_MatFlag200 = 0x200,
	ModelHeaderFlagsE2_MatFlag8 = 0x8,
	ModelHeaderFlagsE2_MatFlag400 = 0x400,
	ModelHeaderFlagsE2_MatFlag4 = 0x4,
	ModelHeaderFlagsE2_MatFlag800 = 0x800,
} ModelHeaderFlagsE2;
//include/sfa/xml/Animation/Model/File.xml
typedef union ModelFileHeader {
	u8 refCount;
	struct __attribute__((packed)) { u8 _pad_flags[0x2]; ModelDataFlags2 flags; };
	struct __attribute__((packed)) { u8 _pad_modelId[0x4]; ModelId modelId; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; dword unk08; };
	struct __attribute__((packed)) { u8 _pad_fileSize[0xC]; u32 fileSize; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; dword unk14; };
	struct __attribute__((packed)) { u8 _pad_flags18[0x18]; float* flags18; };
	struct __attribute__((packed)) { u8 _pad_extraAmapSize[0x1C]; u32 extraAmapSize; };
	struct __attribute__((packed)) { u8 _pad_textures[0x20]; uint* textures; };
	struct __attribute__((packed)) { u8 _pad_flags_0x24[0x24]; ModelDataFlags24 flags_0x24; };
	struct __attribute__((packed)) { u8 _pad_unk25[0x25]; byte unk25; };
	struct __attribute__((packed)) { u8 _pad_vtxs[0x28]; vec3s* vtxs; };
	struct __attribute__((packed)) { u8 _pad_normals[0x2C]; vec3s* normals; };
	struct __attribute__((packed)) { u8 _pad_colors[0x30]; u16* colors; };
	struct __attribute__((packed)) { u8 _pad_texCoords[0x34]; vec2s* texCoords; };
	struct __attribute__((packed)) { u8 _pad_shaders[0x38]; Shader* shaders; };
	struct __attribute__((packed)) { u8 _pad_bones[0x3C]; Bone* bones; };
	struct __attribute__((packed)) { u8 _pad_boneQuats[0x40]; Quaternion* boneQuats; };
	struct __attribute__((packed)) { u8 _pad_unk44[0x44]; u32 unk44; };
	struct __attribute__((packed)) { u8 _pad_unk50[0x50]; u32 unk50; };
	struct __attribute__((packed)) { u8 _pad_vtxGroups[0x54]; ModelVtxGroup* vtxGroups; };
	struct __attribute__((packed)) { u8 _pad_hitSpheres[0x58]; HitSphere* hitSpheres; };
	struct __attribute__((packed)) { u8 _pad_pAltIndBuf[0x64]; u32* pAltIndBuf; };
	struct __attribute__((packed)) { u8 _pad_amapBin[0x68]; pointer amapBin; };
	struct __attribute__((packed)) { u8 _pad_animIds[0x6C]; short* animIds; };
	struct __attribute__((packed)) { u8 _pad_animIdxs[0x70]; ushort animIdxs; };
	struct __attribute__((packed)) { u8 _pad_amapTab[0x80]; u32 amapTab; };
	struct __attribute__((packed)) { u8 _pad_unk84[0x84]; ushort unk84; };
	struct __attribute__((packed)) { u8 _pad_posFineSkinningConfig[0x88]; FineSkinningConfig* posFineSkinningConfig; };
	struct __attribute__((packed)) { u8 _pad_posFineSkinningPieces[0xA4]; FineSkinningPiece* posFineSkinningPieces; };
	struct __attribute__((packed)) { u8 _pad_posFineSkinningWeights[0xA8]; pointer posFineSkinningWeights; };
	struct __attribute__((packed)) { u8 _pad_nrmFineSkinningConfig[0xAC]; FineSkinningConfig* nrmFineSkinningConfig; };
	struct __attribute__((packed)) { u8 _pad_unkB8[0xB8]; dword unkB8; };
	struct __attribute__((packed)) { u8 _pad_bCopyNormalsOnLoad[0xC8]; pointer bCopyNormalsOnLoad; };
	struct __attribute__((packed)) { u8 _pad_unkCC[0xCC]; pointer unkCC; };
	struct __attribute__((packed)) { u8 _pad_dlists[0xD0]; DisplayListPtr* dlists; };
	struct __attribute__((packed)) { u8 _pad_renderInstrs[0xD4]; byte* renderInstrs; };
	struct __attribute__((packed)) { u8 _pad_nRenderInstrs[0xD8]; ushort nRenderInstrs; };
	struct __attribute__((packed)) { u8 _pad_animations[0xDC]; short** animations; };
	struct __attribute__((packed)) { u8 _pad_cullDistance[0xE0]; short cullDistance; };
	struct __attribute__((packed)) { u8 _pad_flagsE2[0xE2]; ModelHeaderFlagsE2 flagsE2; };
	struct __attribute__((packed)) { u8 _pad_nVtxs[0xE4]; short nVtxs; };
	struct __attribute__((packed)) { u8 _pad_nNormals[0xE6]; short nNormals; };
	struct __attribute__((packed)) { u8 _pad_nColors[0xE8]; short nColors; };
	struct __attribute__((packed)) { u8 _pad_nTexCoords[0xEA]; short nTexCoords; };
	struct __attribute__((packed)) { u8 _pad_nAnimations[0xEC]; short nAnimations; };
	struct __attribute__((packed)) { u8 _pad_nTextures[0xF2]; u8 nTextures; };
	struct __attribute__((packed)) { u8 _pad_nBones[0xF3]; u8 nBones; };
	struct __attribute__((packed)) { u8 _pad_nVtxGroups[0xF4]; u8 nVtxGroups; };
	struct __attribute__((packed)) { u8 _pad_nDlists[0xF5]; u8 nDlists; };
	struct __attribute__((packed)) { u8 _pad_unkF6[0xF6]; u8 unkF6; };
	struct __attribute__((packed)) { u8 _pad_nHitSpheres[0xF7]; u8 nHitSpheres; };
	struct __attribute__((packed)) { u8 _pad_nShaders[0xF8]; u8 nShaders; };
	struct __attribute__((packed)) { u8 _pad_nPtrsDC[0xF9]; u8 nPtrsDC; };
	struct __attribute__((packed)) { u8 _pad_nTexMtxs[0xFA]; u8 nTexMtxs; };
	struct __attribute__((packed)) { u8 _pad_unkFB[0xFB]; u8 unkFB; };
} ModelFileHeader;
//include/sfa/xml/Animation/Model/File.xml
typedef union ModelListEntry {
	ModelId modelId;
	struct __attribute__((packed)) { u8 _pad_data[0x2]; ModelFileHeader* data; };
} ModelListEntry;
//include/sfa/xml/Animation/Model/File.xml
typedef union ModelsBinEntry {
	u32 signature;
	struct __attribute__((packed)) { u8 _pad_size[0x4]; u32 size; };
	struct __attribute__((packed)) { u8 _pad_noAmap[0x18]; undefined4 noAmap; };
	struct __attribute__((packed)) { u8 _pad_nAnimations[0x1C]; undefined4 nAnimations; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; undefined4 unk20; };
	struct __attribute__((packed)) { u8 _pad_zlb[0x24]; ZlbHeader zlb; };
} ModelsBinEntry;
