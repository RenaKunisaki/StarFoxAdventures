typedef enum { //type:u8
    ASSET_TYPE_FILE              = 0x0,
    ASSET_TYPE_FILE_WITH_ID      = 0x1,
    ASSET_TYPE_FILE_WITH_ID_SIZE = 0x2,
    ASSET_TYPE_TEXTURE           = 0x3,
	ASSET_TYPE_CHARACTER         = 0x4,
	ASSET_TYPE_DLL               = 0x5,
	ASSET_TYPE_DUMMY             = 0x6, //ModelInstance, not used in final
	ASSET_TYPE_ANIMATION         = 0x7,
} AssetTypeEnum;

typedef struct PACKED AssetDef {
    u8         unk00;      //0x00
	u8         type;       //0x01 AssetTypeEnum
	u32        dataFileId; //0x04 DataFileEnum
	void      *obj;        //0x08
	int       *buffer;     //0x0C
	uint       size;       //0x10
	uint       offset;     //0x14
	int        gameObj;    //0x18
	ObjDef    *object;     //0x1C
	uint       objFlags;   //0x20
	undefined4 dest;       //0x24
	undefined4 buffer2;    //0x28
} AssetDef;
