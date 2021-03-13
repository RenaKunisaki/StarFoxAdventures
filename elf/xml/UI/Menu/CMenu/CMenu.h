//include/sfa/xml/UI/Menu/CMenu.xml
typedef union CMenuItem {
	GameBit16 itemBit;
	struct __attribute__((packed)) { u8 _pad_hideBit[0x2]; GameBit16 hideBit; };
	struct __attribute__((packed)) { u8 _pad_disableBit[0x4]; GameBit16 disableBit; };
	struct __attribute__((packed)) { u8 _pad_textureId[0x6]; short textureId; };
	struct __attribute__((packed)) { u8 _pad_texture2[0x8]; short texture2; };
	struct __attribute__((packed)) { u8 _pad_unk0A[0xA]; short unk0A; };
	struct __attribute__((packed)) { u8 _pad_text[0xC]; GameTextId text; };
	struct __attribute__((packed)) { u8 _pad_unk0E[0xE]; byte unk0E; };
	struct __attribute__((packed)) { u8 _pad_unk0F[0xF]; byte unk0F; };
} CMenuItem;
//include/sfa/xml/UI/Menu/CMenu.xml
typedef union CMenuPageItems {
	CMenuItem* items;
	struct __attribute__((packed)) { u8 _pad_selected[0x4]; undefined2 selected; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; undefined4 unk08; };
} CMenuPageItems;
