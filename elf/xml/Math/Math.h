//include/sfa/xml/Math.xml
typedef union BoundingBox3f {
	float x1;
	struct __attribute__((packed)) { u8 _pad_x2[0x4]; float x2; };
	struct __attribute__((packed)) { u8 _pad_y1[0x8]; float y1; };
	struct __attribute__((packed)) { u8 _pad_y2[0xC]; float y2; };
	struct __attribute__((packed)) { u8 _pad_z1[0x10]; float z1; };
	struct __attribute__((packed)) { u8 _pad_z2[0x14]; float z2; };
} BoundingBox3f;
