//include/sfa/xml/Debug/Light.xml
typedef enum { //type:u8
	LightAdjustFlags_DEC_MODE = 0x4,
	LightAdjustFlags_DEC_VALUE = 0x1,
	LightAdjustFlags_INC_VALUE = 0x2,
	LightAdjustFlags_TOGGLE_FIELD_0xC = 0x10,
	LightAdjustFlags_INC_MODE = 0x8,
} LightAdjustFlags;
//include/sfa/xml/Debug/Light.xml
typedef enum { //type:u8
	LightModeEnum_DIFFUSE_GREEN = 0x3,
	LightModeEnum_DIFFUSE_RED = 0x2,
	LightModeEnum_DIFFUSE_BLUE = 0x4,
	LightModeEnum_SPECULAR_GREEN = 0x6,
	LightModeEnum_SPECULAR_RED = 0x5,
	LightModeEnum_PITCH = 0x1,
	LightModeEnum_SPECULAR_BLUE = 0x7,
	LightModeEnum_YAW = 0x0,
} LightModeEnum;
