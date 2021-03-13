typedef void (*objBeforeMoveFn)(ObjInstance* param1);
typedef undefined (*objField108_func)(ObjInstance* param1, Model* param2, Mtx44* param3);
typedef undefined4 (*ObjFunc)();
typedef int (*objGetExtraSizeFn)(ObjInstance* param1, int param2);
typedef void (*objModelMtxFn28)(ObjInstance* param1, undefined4 param2, vec3f* param3);
typedef void (*objOnFreeDefFn)(ObjInstance* param1, undefined4 param2);
typedef void (*objOnLoadFn)(ObjInstance* param1);
typedef void (*objRenderFn)(ObjInstance* param1, undefined4 param2, undefined4 param3, undefined4 param4, undefined4 param5, BOOL param6);
typedef void (*objUpdateFn)(ObjInstance* param1);
//include/sfa/xml/Game/GameObject/Functions.xml
typedef union ObjectFuncPtrs {
	ObjectFuncPtrs2* ptrs2;
} ObjectFuncPtrs;
//include/sfa/xml/Game/GameObject/Functions.xml
typedef union ObjectFuncPtrs2 {
	pointer unk00;
	struct __attribute__((packed)) { u8 _pad_onLoad[0x4]; objOnLoadFn* onLoad; };
	struct __attribute__((packed)) { u8 _pad_update[0x8]; objUpdateFn* update; };
	struct __attribute__((packed)) { u8 _pad_hitDetect[0xC]; objBeforeMoveFn* hitDetect; };
	struct __attribute__((packed)) { u8 _pad_render[0x10]; objRenderFn* render; };
	struct __attribute__((packed)) { u8 _pad_onFreeObjDef[0x14]; objOnFreeDefFn* onFreeObjDef; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; pointer unk18; };
	struct __attribute__((packed)) { u8 _pad_getExtraSize[0x1C]; objGetExtraSizeFn* getExtraSize; };
	struct __attribute__((packed)) { u8 _pad_setScale[0x20]; pointer setScale; };
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; pointer unk24; };
	struct __attribute__((packed)) { u8 _pad_modelMtxFn_0x28[0x28]; objModelMtxFn28* modelMtxFn_0x28; };
	struct __attribute__((packed)) { u8 _pad_render2C[0x2C]; pointer render2C; };
} ObjectFuncPtrs2;
