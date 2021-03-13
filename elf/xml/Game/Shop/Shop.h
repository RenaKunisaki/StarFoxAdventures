//include/sfa/xml/Game/Shop.xml
typedef union BuyItemStruct2 {
	struct __attribute__((packed)) { u8 _pad_price[0x1A]; short price; };
} BuyItemStruct2;
//include/sfa/xml/Game/Shop.xml
typedef union ShopItem {
	u8 price;
	struct __attribute__((packed)) { u8 _pad_minPrice[0x1]; u8 minPrice; };
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; byte unk04; };
	struct __attribute__((packed)) { u8 _pad_curMinPrice[0x5]; u8 curMinPrice; };
	struct __attribute__((packed)) { u8 _pad_whenAvailable[0x6]; GameBit16 whenAvailable; };
	struct __attribute__((packed)) { u8 _pad_isBought[0x8]; GameBit16 isBought; };
	struct __attribute__((packed)) { u8 _pad_textId[0xA]; u16 textId; };
} ShopItem;
