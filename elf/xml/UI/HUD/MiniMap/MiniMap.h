//include/sfa/xml/UI/HUD/MiniMap.xml
typedef enum { //type:u8
	MinimapMode_FuelCellCompass = 0x1,
	MinimapMode_Information = 0x2,
	MinimapMode_Map = 0x0,
} MinimapMode;
//include/sfa/xml/UI/HUD/MiniMap.xml
typedef union MinimapLocation {
	MinimapRegion* region;
	struct __attribute__((packed)) { u8 _pad_mapBit[0x4]; GameBit16 mapBit; };
	struct __attribute__((packed)) { u8 _pad_mapId[0x6]; MapId8 mapId; };
	struct __attribute__((packed)) { u8 _pad_unk07[0x7]; byte unk07; };
} MinimapLocation;
//include/sfa/xml/UI/HUD/MiniMap.xml
typedef union MinimapRegion {
	undefined2 xMin;
	struct __attribute__((packed)) { u8 _pad_xMax[0x2]; undefined2 xMax; };
	struct __attribute__((packed)) { u8 _pad_zMin[0x4]; undefined2 zMin; };
	struct __attribute__((packed)) { u8 _pad_zMax[0x6]; undefined2 zMax; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; undefined2 unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0A[0xA]; undefined2 unk0A; };
	struct __attribute__((packed)) { u8 _pad_whenAvailable[0xC]; GameBit16 whenAvailable; };
	struct __attribute__((packed)) { u8 _pad_unk0E[0xE]; undefined1 unk0E; };
	struct __attribute__((packed)) { u8 _pad_unk0F[0xF]; undefined1 unk0F; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; undefined2 unk10; };
	struct __attribute__((packed)) { u8 _pad_isRotated[0x12]; undefined1 isRotated; };
} MinimapRegion;
