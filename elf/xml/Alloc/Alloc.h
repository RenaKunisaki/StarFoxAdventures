//include/sfa/xml/Alloc.xml
typedef enum { //type:u32
	AllocTag_CAMERA_PARAMS = 0xF,
	AllocTag_WATERFX = 0x13,
	AllocTag_COMPRESSED_FILE = 0x7F7F7FFF,
	AllocTag_UNKNOWN_32BYTE_BUFFER = 0xFF,
	AllocTag_MODELSTRUCT = 0x1A,
	AllocTag_OBJECTS = 0xE,
	AllocTag_SAVEGAME = 0xFFFFFFFF,
	AllocTag_AUDIO = 0xB,
	AllocTag_VEC3F_ARRAY = 0x18,
	AllocTag_SCREENS_BIN = 0x2,
	AllocTag_STACK = 0x11,
	AllocTag_MAP_BLOCKS = 0x5,
	AllocTag_TEXTURE = 0x6,
	AllocTag_FACEFEED = 0xFACEFEED,
	AllocTag_GENERIC = 0x0,
	AllocTag_MODELS = 0xA,
	AllocTag_INTERSECT_POINT = 0xFFFF00FF,
	AllocTag_FILE = 0x7D7D7D7D,
	AllocTag_MODEL_DATA = 0x9,
	AllocTag_VOX = 0x10,
} AllocTag;
//include/sfa/xml/Alloc.xml
typedef enum { //type:u32
	MemDefragOp_DefragTextures = 0x0,
	MemDefragOp_DefragTextureFiles = 0x2,
	MemDefragOp_DefragMapAssets = 0x1,
} MemDefragOp;
//include/sfa/xml/Alloc.xml
typedef union heapStruct {
	int avail;
	struct __attribute__((packed)) { u8 _pad_used[0x4]; int used; };
	struct __attribute__((packed)) { u8 _pad_data[0x8]; SfaHeapEntry* data; };
	struct __attribute__((packed)) { u8 _pad_usage[0xC]; int usage; };
	struct __attribute__((packed)) { u8 _pad_size[0x10]; int size; };
} heapStruct;
//include/sfa/xml/Alloc.xml
typedef union SfaHeap {
	uint dataSize;
	struct __attribute__((packed)) { u8 _pad_size[0x4]; dword size; };
	struct __attribute__((packed)) { u8 _pad_avail[0x8]; dword avail; };
	struct __attribute__((packed)) { u8 _pad_used[0xC]; dword used; };
	struct __attribute__((packed)) { u8 _pad_data[0x10]; SfaHeapEntry* data; };
} SfaHeap;
//include/sfa/xml/Alloc.xml
typedef union SfaHeapEntry {
	pointer loc;
	struct __attribute__((packed)) { u8 _pad_size[0x4]; int size; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; undefined2 unk08; };
	struct __attribute__((packed)) { u8 _pad_idx[0xA]; short idx; };
	struct __attribute__((packed)) { u8 _pad_stack2[0xC]; short stack2; };
	struct __attribute__((packed)) { u8 _pad_stack[0xE]; short stack; };
	struct __attribute__((packed)) { u8 _pad_col[0x10]; AllocTag col; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; int unk14; };
	struct __attribute__((packed)) { u8 _pad_mmUniqueIdent[0x18]; int mmUniqueIdent; };
} SfaHeapEntry;
