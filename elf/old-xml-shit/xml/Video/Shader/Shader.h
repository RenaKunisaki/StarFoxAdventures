//include/sfa/xml/Video/Shader.xml
typedef enum { //type:u8
	ShaderAttrFlags_UseNormals = 0x1,
	ShaderAttrFlags_UseColors = 0x2,
} ShaderAttrFlags;
//include/sfa/xml/Video/Shader.xml
typedef enum { //type:u32
	ShaderFlags_Water = 0x80000000,
	ShaderFlags_DevGeometry = 0x2,
	ShaderFlags_FuzzRelated = 0x200,
	ShaderFlags_Lava = 0x80,
	ShaderFlags_MediumFur = 0x8000,
	ShaderFlags_LongFur = 0x10000,
	ShaderFlags_ShortFur = 0x4000,
	ShaderFlags_IndoorOutdoorBlend = 0x40000,
	ShaderFlags_ReflectSkyscape = 0x20,
	ShaderFlags_AlphaCompare = 0x400,
	ShaderFlags_StreamingVideo = 0x20000,
	ShaderFlags_ForceBlend = 0x40000000,
	ShaderFlags_Reflective = 0x100,
	ShaderFlags_Caustic = 0x40,
	ShaderFlags_CullBackface = 0x8,
	ShaderFlags_Fog = 0x4,
} ShaderFlags;
//include/sfa/xml/Video/Shader.xml
typedef union ShaderLayer {
	pointer texture;
	struct __attribute__((packed)) { u8 _pad_tevMode[0x4]; u8 tevMode; };
	struct __attribute__((packed)) { u8 _pad_enableTexChainStuff[0x5]; u8 enableTexChainStuff; };
	struct __attribute__((packed)) { u8 _pad_scrollingTexMtx[0x6]; u8 scrollingTexMtx; };
} ShaderLayer;
//include/sfa/xml/Video/Shader.xml
typedef union Shader {
	struct __attribute__((packed)) { u8 _pad_r[0x4]; byte r; };
	struct __attribute__((packed)) { u8 _pad_g[0x5]; byte g; };
	struct __attribute__((packed)) { u8 _pad_b[0x6]; byte b; };
	struct __attribute__((packed)) { u8 _pad_auxTex0[0x8]; pointer auxTex0; };
	struct __attribute__((packed)) { u8 _pad_alpha[0xC]; byte alpha; };
	struct __attribute__((packed)) { u8 _pad_auxTex1[0x14]; pointer auxTex1; };
	struct __attribute__((packed)) { u8 _pad_texture18[0x18]; pointer texture18; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; int unk1C; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; pointer unk20; };
	struct __attribute__((packed)) { u8 _pad_layer[0x24]; ShaderLayer layer; };
	struct __attribute__((packed)) { u8 _pad_auxTex2[0x34]; pointer auxTex2; };
	struct __attribute__((packed)) { u8 _pad_furTexture[0x38]; pointer furTexture; };
	struct __attribute__((packed)) { u8 _pad_flags[0x3C]; ShaderFlags flags; };
	struct __attribute__((packed)) { u8 _pad_attrFlags[0x40]; ShaderAttrFlags attrFlags; };
	struct __attribute__((packed)) { u8 _pad_nLayers[0x41]; u8 nLayers; };
	struct __attribute__((packed)) { u8 _pad_unk42[0x42]; u8 unk42; };
	struct __attribute__((packed)) { u8 _pad_unk43[0x43]; u8 unk43; };
} Shader;
//include/sfa/xml/Video/Shader.xml
typedef union ShaderColor {
	u8 flags;
	struct __attribute__((packed)) { u8 _pad_maybeNumTextures[0x1]; u8 maybeNumTextures; };
	struct __attribute__((packed)) { u8 _pad_unk02[0x2]; u8 unk02; };
	struct __attribute__((packed)) { u8 _pad_textureId[0x3]; u8 textureId; };
	struct __attribute__((packed)) { u8 _pad_color[0x4]; int color; };
} ShaderColor;
//include/sfa/xml/Video/Shader.xml
typedef union ShaderDef {
	pointer texture;
	struct __attribute__((packed)) { u8 _pad_lighting[0x4]; pointer lighting; };
	struct __attribute__((packed)) { u8 _pad_material[0x8]; pointer material; };
} ShaderDef;
