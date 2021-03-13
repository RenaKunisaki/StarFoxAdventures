//include/sfa/xml/Game/GameObject/ObjInstance.xml
typedef enum { //type:u16
	ObjInstance_Flags06_DontTrackOldPositions = 0x8,
	ObjInstance_Flags06_Invisible = 0x4000,
	ObjInstance_Flags06_DontSave = 0x2000,
} ObjInstance_Flags06;
//include/sfa/xml/Game/GameObject/ObjInstance.xml
typedef enum { //type:u8
	ObjInstance_FlagsAF_ModelDisableFlag20 = 0x20,
	ObjInstance_FlagsAF_FieldB8TimeCanBeNegative = 0x4,
	ObjInstance_FlagsAF_CantBeUsed = 0x10,
	ObjInstance_FlagsAF_CanPressAToTalk = 0x1,
	ObjInstance_FlagsAF_ModelDisableFlag08 = 0x8,
} ObjInstance_FlagsAF;
//include/sfa/xml/Game/GameObject/ObjInstance.xml
typedef enum { //type:u16
	ObjInstance_FlagsB0_DontMove = 0x2000,
	ObjInstance_FlagsB0_ScalingFlag08 = 0x8,
	ObjInstance_FlagsB0_IsRendered = 0x800,
	ObjInstance_FlagsB0_WhichParentHitbox = 0x7,
	ObjInstance_FlagsB0_LockAnimsAndControls = 0x80,
	ObjInstance_FlagsB0_FallThruFloor = 0x20,
	ObjInstance_FlagsB0_CanFree = 0x10,
	ObjInstance_FlagsB0_Invisible = 0x400,
	ObjInstance_FlagsB0_DontUpdate = 0x8000,
	ObjInstance_FlagsB0_DontUseRenderCallback = 0x4000,
	ObjInstance_FlagsB0_IsFreed = 0x40,
	ObjInstance_FlagsB0_SeqActive = 0x1000,
} ObjInstance_FlagsB0;
//include/sfa/xml/Game/GameObject/ObjInstance.xml
typedef union ObjPos {
	vec3s rotation;
	struct __attribute__((packed)) { u8 _pad_flags[0x6]; ObjInstance_Flags06 flags; };
	struct __attribute__((packed)) { u8 _pad_scale[0x8]; float scale; };
	struct __attribute__((packed)) { u8 _pad_pos[0xC]; vec3f pos; };
} ObjPos;
//include/sfa/xml/Game/GameObject/ObjInstance.xml
typedef union ObjInstance {
	ObjPos pos;
	struct __attribute__((packed)) { u8 _pad_prevPos[0x18]; vec3f prevPos; };
	struct __attribute__((packed)) { u8 _pad_vel[0x24]; vec3f vel; };
	struct __attribute__((packed)) { u8 _pad_pMatrix[0x30]; ObjInstance* pMatrix; };
	struct __attribute__((packed)) { u8 _pad_map[0x34]; MapId8 map; };
	struct __attribute__((packed)) { u8 _pad_mtxIdx[0x35]; u8 mtxIdx; };
	struct __attribute__((packed)) { u8 _pad_newOpacity[0x36]; u8 newOpacity; };
	struct __attribute__((packed)) { u8 _pad_opacity[0x37]; u8 opacity; };
	struct __attribute__((packed)) { u8 _pad_next[0x38]; ObjInstance* next; };
	struct __attribute__((packed)) { u8 _pad_camDistVar3C[0x3C]; float camDistVar3C; };
	struct __attribute__((packed)) { u8 _pad_camDistVar40[0x40]; float camDistVar40; };
	struct __attribute__((packed)) { u8 _pad_catId[0x44]; ObjCatId catId; };
	struct __attribute__((packed)) { u8 _pad_defNo[0x46]; ObjDefEnum defNo; };
	struct __attribute__((packed)) { u8 _pad_defNo48[0x48]; short defNo48; };
	struct __attribute__((packed)) { u8 _pad_objDef[0x4C]; ObjDef* objDef; };
	struct __attribute__((packed)) { u8 _pad_file[0x50]; ObjectFileStruct* file; };
	struct __attribute__((packed)) { u8 _pad_hitstate[0x54]; HitState* hitstate; };
	struct __attribute__((packed)) { u8 _pad_hitboxMtx[0x58]; HitboxMatrix* hitboxMtx; };
	struct __attribute__((packed)) { u8 _pad_unk5C[0x5C]; dword* unk5C; };
	struct __attribute__((packed)) { u8 _pad_pEventName[0x60]; pointer pEventName; };
	struct __attribute__((packed)) { u8 _pad_shadow[0x64]; Shadow* shadow; };
	struct __attribute__((packed)) { u8 _pad_dll[0x68]; ObjectFuncPtrs* dll; };
	struct __attribute__((packed)) { u8 _pad_pVecs[0x6C]; pointer pVecs; };
	struct __attribute__((packed)) { u8 _pad_pTextures[0x70]; astruct_53** pTextures; };
	struct __attribute__((packed)) { u8 _pad_focusPoints[0x74]; vec3f focusPoints; };
	struct __attribute__((packed)) { u8 _pad_unk78[0x78]; pointer unk78; };
	struct __attribute__((packed)) { u8 _pad_models[0x7C]; Model** models; };
	struct __attribute__((packed)) { u8 _pad_oldPos[0x80]; vec3f oldPos; };
	struct __attribute__((packed)) { u8 _pad_pos_0x8c[0x8C]; vec3f pos_0x8c; };
	struct __attribute__((packed)) { u8 _pad_animTimer[0x98]; float animTimer; };
	struct __attribute__((packed)) { u8 _pad_animVal_9c[0x9C]; float animVal_9c; };
	struct __attribute__((packed)) { u8 _pad_animId[0xA0]; word animId; };
	struct __attribute__((packed)) { u8 _pad_animVal_a2[0xA2]; GameBit16 animVal_a2; };
	struct __attribute__((packed)) { u8 _pad_cullOffset[0xA4]; float cullOffset; };
	struct __attribute__((packed)) { u8 _pad_cullDistance[0xA8]; float cullDistance; };
	struct __attribute__((packed)) { u8 _pad_mapId[0xAC]; MapId8 mapId; };
	struct __attribute__((packed)) { u8 _pad_curModel[0xAD]; byte curModel; };
	struct __attribute__((packed)) { u8 _pad_slot[0xAE]; u8 slot; };
	struct __attribute__((packed)) { u8 _pad_flags_0xaf[0xAF]; ObjInstance_FlagsAF flags_0xaf; };
	struct __attribute__((packed)) { u8 _pad_flags_0xb0[0xB0]; ObjInstance_FlagsB0 flags_0xb0; };
	struct __attribute__((packed)) { u8 _pad_objNo[0xB2]; s16 objNo; };
	struct __attribute__((packed)) { u8 _pad_curSeq[0xB4]; short curSeq; };
	struct __attribute__((packed)) { u8 _pad_state[0xB8]; pointer state; };
	struct __attribute__((packed)) { u8 _pad_seqFn[0xBC]; ObjSeqFn* seqFn; };
	struct __attribute__((packed)) { u8 _pad_copyMtxFrom[0xC0]; ObjInstance* copyMtxFrom; };
	struct __attribute__((packed)) { u8 _pad_parent[0xC4]; ObjInstance* parent; };
	struct __attribute__((packed)) { u8 _pad_child[0xC8]; ObjInstance* child; };
	struct __attribute__((packed)) { u8 _pad_unkD4[0xD4]; byte* unkD4; };
	struct __attribute__((packed)) { u8 _pad_unkD8[0xD8]; word unkD8; };
	struct __attribute__((packed)) { u8 _pad_seqMsgs[0xDC]; ObjSeqMsgQueue* seqMsgs; };
	struct __attribute__((packed)) { u8 _pad_flag_altMtrlColor[0xE3]; int flag_altMtrlColor; };
	struct __attribute__((packed)) { u8 _pad_flags_e3[0xE3]; int flags_e3; };
	struct __attribute__((packed)) { u8 _pad_unkE4[0xE4]; byte unkE4; };
	struct __attribute__((packed)) { u8 _pad_flags_0xe5[0xE5]; byte flags_0xe5; };
	struct __attribute__((packed)) { u8 _pad_unkE6[0xE6]; short unkE6; };
	struct __attribute__((packed)) { u8 _pad_hintTextIdx[0xE8]; byte hintTextIdx; };
	struct __attribute__((packed)) { u8 _pad_unkEA[0xEA]; byte unkEA; };
	struct __attribute__((packed)) { u8 _pad_nChildren[0xEB]; u8 nChildren; };
	struct __attribute__((packed)) { u8 _pad_colorEC[0xEC]; Color4b colorEC; };
	struct __attribute__((packed)) { u8 _pad_unkF0[0xF0]; byte unkF0; };
	struct __attribute__((packed)) { u8 _pad_brightness[0xF1]; byte brightness; };
	struct __attribute__((packed)) { u8 _pad_colorIdx[0xF2]; byte colorIdx; };
	struct __attribute__((packed)) { u8 _pad_curveNoPlus1[0xF4]; undefined4 curveNoPlus1; };
	struct __attribute__((packed)) { u8 _pad_flags_0xf8[0xF8]; dword flags_0xf8; };
	struct __attribute__((packed)) { u8 _pad_oldVel[0xFC]; vec3f oldVel; };
	struct __attribute__((packed)) { u8 _pad_cbAfterUpdateBones[0x108]; objField108_func* cbAfterUpdateBones; };
} ObjInstance;
