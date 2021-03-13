//include/sfa/xml/Animation/Model/Animation/Skeleton.xml
typedef union Bone {
	s8 parent;
	struct __attribute__((packed)) { u8 _pad_idx[0x1]; byte idx; };
	struct __attribute__((packed)) { u8 _pad_translation[0x4]; vec3f translation; };
	struct __attribute__((packed)) { u8 _pad_bindTranslation[0x10]; vec3f bindTranslation; };
} Bone;
//include/sfa/xml/Animation/Model/Animation/Skeleton.xml
typedef union BoneStruct_59 {
	float** pHead;
	struct __attribute__((packed)) { u8 _pad_pTail[0x4]; float** pTail; };
	struct __attribute__((packed)) { u8 _pad_maybeHead[0x8]; vec3f maybeHead; };
	struct __attribute__((packed)) { u8 _pad_maybeTail[0x14]; vec3f maybeTail; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; vec3f unk20; };
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; float* unk2C; };
	struct __attribute__((packed)) { u8 _pad_unk30[0x30]; float unk30; };
	struct __attribute__((packed)) { u8 _pad_dist[0x34]; float* dist; };
	struct __attribute__((packed)) { u8 _pad_unk38[0x38]; float* unk38; };
	struct __attribute__((packed)) { u8 _pad_oneOverField30[0x3C]; float* oneOverField30; };
	struct __attribute__((packed)) { u8 _pad_iBone[0x40]; int iBone; };
	struct __attribute__((packed)) { u8 _pad_iMtx[0x44]; float* iMtx; };
} BoneStruct_59;
//include/sfa/xml/Animation/Model/Animation/Skeleton.xml
typedef union BoneTransformResult {
	short* next;
	struct __attribute__((packed)) { u8 _pad_pos[0x4]; vec3s pos; };
} BoneTransformResult;
