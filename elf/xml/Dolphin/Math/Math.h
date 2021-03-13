//include/sfa/xml/Dolphin/Math.xml
typedef union Mtx34 {
	float m11;
	struct __attribute__((packed)) { u8 _pad_m12[0x4]; float m12; };
	struct __attribute__((packed)) { u8 _pad_m13[0x8]; float m13; };
	struct __attribute__((packed)) { u8 _pad_m21[0xC]; float m21; };
	struct __attribute__((packed)) { u8 _pad_m22[0x10]; float m22; };
	struct __attribute__((packed)) { u8 _pad_m23[0x14]; float m23; };
	struct __attribute__((packed)) { u8 _pad_m31[0x18]; float m31; };
	struct __attribute__((packed)) { u8 _pad_m32[0x1C]; float m32; };
	struct __attribute__((packed)) { u8 _pad_m33[0x20]; float m33; };
	struct __attribute__((packed)) { u8 _pad_m41[0x24]; float m41; };
	struct __attribute__((packed)) { u8 _pad_m42[0x28]; float m42; };
	struct __attribute__((packed)) { u8 _pad_m43[0x2C]; float m43; };
} Mtx34;
//include/sfa/xml/Dolphin/Math.xml
typedef union Mtx43 {
	float m11;
	struct __attribute__((packed)) { u8 _pad_m12[0x4]; float m12; };
	struct __attribute__((packed)) { u8 _pad_m13[0x8]; float m13; };
	struct __attribute__((packed)) { u8 _pad_m14[0xC]; float m14; };
	struct __attribute__((packed)) { u8 _pad_m21[0x10]; float m21; };
	struct __attribute__((packed)) { u8 _pad_m22[0x14]; float m22; };
	struct __attribute__((packed)) { u8 _pad_m23[0x18]; float m23; };
	struct __attribute__((packed)) { u8 _pad_m24[0x1C]; float m24; };
	struct __attribute__((packed)) { u8 _pad_m31[0x20]; float m31; };
	struct __attribute__((packed)) { u8 _pad_m32[0x24]; float m32; };
	struct __attribute__((packed)) { u8 _pad_m33[0x28]; float m33; };
	struct __attribute__((packed)) { u8 _pad_m34[0x2C]; float m34; };
} Mtx43;
//include/sfa/xml/Dolphin/Math.xml
typedef union vec2s {
	short x;
	struct __attribute__((packed)) { u8 _pad_y[0x2]; short y; };
} vec2s;
//include/sfa/xml/Dolphin/Math.xml
typedef union vec3f {
	float x;
	struct __attribute__((packed)) { u8 _pad_y[0x4]; float y; };
	struct __attribute__((packed)) { u8 _pad_z[0x8]; float z; };
} vec3f;
//include/sfa/xml/Dolphin/Math.xml
typedef union vec3i {
	int x;
	struct __attribute__((packed)) { u8 _pad_y[0x4]; int y; };
	struct __attribute__((packed)) { u8 _pad_z[0x8]; int z; };
} vec3i;
//include/sfa/xml/Dolphin/Math.xml
typedef union vec3s {
	short x;
	struct __attribute__((packed)) { u8 _pad_y[0x2]; short y; };
	struct __attribute__((packed)) { u8 _pad_z[0x4]; short z; };
} vec3s;
