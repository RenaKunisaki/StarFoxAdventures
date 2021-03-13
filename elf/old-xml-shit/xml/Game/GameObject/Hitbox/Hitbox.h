//include/sfa/xml/Game/GameObject/Hitbox.xml
typedef enum { //type:u16
	HitboxFlags60_NeedPosUpdate = 0x40,
	HitboxFlags60_LockRotZ = 0x1000,
	HitboxFlags60_LockRotY = 0x800,
	HitboxFlags60_Disabled = 0x1,
} HitboxFlags60;
//include/sfa/xml/Game/GameObject/Hitbox.xml
typedef enum { //type:u8
	HitboxFlags62_UseModelField14 = 0x20,
	HitboxFlags62_DontUpdate = 0x8,
	HitboxFlags62_ScaleBySize = 0x1,
	HitboxFlags62_SizeFlag2 = 0x2,
} HitboxFlags62;
//include/sfa/xml/Game/GameObject/Hitbox.xml
typedef union HitboxMatrix {
	Mtx44 mtx;
	struct __attribute__((packed)) { u8 _pad_mtxIdx[0x10C]; byte mtxIdx; };
	struct __attribute__((packed)) { u8 _pad_rotCnt[0x10D]; char rotCnt; };
	struct __attribute__((packed)) { u8 _pad_unk10F[0x10F]; byte unk10F; };
} HitboxMatrix;
//include/sfa/xml/Game/GameObject/Hitbox.xml
typedef union HitsBinEntry {
	short unk00;
	struct __attribute__((packed)) { u8 _pad_unk02[0x2]; short unk02; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; short unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0A[0xA]; short unk0A; };
	struct __attribute__((packed)) { u8 _pad_unk0F[0xF]; byte unk0F; };
} HitsBinEntry;
//include/sfa/xml/Game/GameObject/Hitbox.xml
typedef union HitSphere {
	u16 bone;
	struct __attribute__((packed)) { u8 _pad_unk02[0x2]; u16 unk02; };
	struct __attribute__((packed)) { u8 _pad_radius[0x4]; float radius; };
	struct __attribute__((packed)) { u8 _pad_pos[0x8]; vec3f pos; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; ushort unk14; };
	struct __attribute__((packed)) { u8 _pad_unk16[0x16]; char unk16; };
	struct __attribute__((packed)) { u8 _pad_unk17[0x17]; char unk17; };
} HitSphere;
//include/sfa/xml/Game/GameObject/Hitbox.xml
typedef union HitSphere_conflict {
	SoundId soundId_00;
	struct __attribute__((packed)) { u8 _pad_soundId_02[0x2]; SoundId soundId_02; };
	struct __attribute__((packed)) { u8 _pad_animId_04[0x4]; s16 animId_04; };
	struct __attribute__((packed)) { u8 _pad_unk06[0x6]; s16 unk06; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; byte unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; float unk0C; };
} HitSphere_conflict;
//include/sfa/xml/Game/GameObject/Hitbox.xml
typedef union HitSpherePos {
	float radius;
	struct __attribute__((packed)) { u8 _pad_pos[0x4]; vec3f pos; };
} HitSpherePos;
//include/sfa/xml/Game/GameObject/Hitbox.xml
typedef union HitState {
	ObjInstance* curTouchedObject;
	struct __attribute__((packed)) { u8 _pad_objHitsSize[0x4]; short objHitsSize; };
	struct __attribute__((packed)) { u8 _pad_maxHitsSize[0x6]; short maxHitsSize; };
	struct __attribute__((packed)) { u8 _pad_objHits[0x8]; ObjHitsEntry* objHits; };
	struct __attribute__((packed)) { u8 _pad_sizeXY_0C[0xC]; float sizeXY_0C; };
	struct __attribute__((packed)) { u8 _pad_pos[0x10]; vec3f pos; };
	struct __attribute__((packed)) { u8 _pad_prevPos[0x1C]; vec3f prevPos; };
	struct __attribute__((packed)) { u8 _pad_size[0x28]; vec3f size; };
	struct __attribute__((packed)) { u8 _pad_minSizeY[0x34]; float minSizeY; };
	struct __attribute__((packed)) { u8 _pad_sizeY[0x38]; float sizeY; };
	struct __attribute__((packed)) { u8 _pad_unk3C[0x3C]; vec3f unk3C; };
	struct __attribute__((packed)) { u8 _pad_unk48[0x48]; undefined4 unk48; };
	struct __attribute__((packed)) { u8 _pad_unk4C[0x4C]; dword unk4C; };
	struct __attribute__((packed)) { u8 _pad_lastTouchedObject[0x50]; ObjInstance* lastTouchedObject; };
	struct __attribute__((packed)) { u8 _pad_lowestDist[0x58]; u16 lowestDist; };
	struct __attribute__((packed)) { u8 _pad_sizeXY[0x5A]; short sizeXY; };
	struct __attribute__((packed)) { u8 _pad_sizeX1[0x5C]; short sizeX1; };
	struct __attribute__((packed)) { u8 _pad_sizeX2[0x5E]; short sizeX2; };
	struct __attribute__((packed)) { u8 _pad_flags_0x60[0x60]; HitboxFlags60 flags_0x60; };
	struct __attribute__((packed)) { u8 _pad_flags_0x62[0x62]; HitboxFlags62 flags_0x62; };
	struct __attribute__((packed)) { u8 _pad_sizeZ[0x64]; short sizeZ; };
	struct __attribute__((packed)) { u8 _pad_sizeZ1[0x66]; short sizeZ1; };
	struct __attribute__((packed)) { u8 _pad_sizeZ2[0x68]; short sizeZ2; };
	struct __attribute__((packed)) { u8 _pad_unk6A[0x6A]; byte unk6A; };
	struct __attribute__((packed)) { u8 _pad_unk6B[0x6B]; undefined1 unk6B; };
	struct __attribute__((packed)) { u8 _pad_damage[0x6C]; byte damage; };
	struct __attribute__((packed)) { u8 _pad_unk6D[0x6D]; byte unk6D; };
	struct __attribute__((packed)) { u8 _pad_unk6E[0x6E]; byte unk6E; };
	struct __attribute__((packed)) { u8 _pad_unk6F[0x6F]; byte unk6F; };
	struct __attribute__((packed)) { u8 _pad_unk70[0x70]; byte unk70; };
	struct __attribute__((packed)) { u8 _pad_nHits[0x71]; byte nHits; };
	struct __attribute__((packed)) { u8 _pad_sphereIdxs[0x72]; byte sphereIdxs; };
	struct __attribute__((packed)) { u8 _pad_objField6C[0x75]; byte objField6C; };
	struct __attribute__((packed)) { u8 _pad_recordedDamage[0x78]; byte recordedDamage; };
	struct __attribute__((packed)) { u8 _pad_objs[0x7C]; ObjInstance* objs; };
	struct __attribute__((packed)) { u8 _pad_objX[0x88]; float objX; };
	struct __attribute__((packed)) { u8 _pad_objY[0x94]; float objY; };
	struct __attribute__((packed)) { u8 _pad_objZ[0xA0]; float objZ; };
	struct __attribute__((packed)) { u8 _pad_typeOfPolyHit[0xAC]; PolygonType typeOfPolyHit; };
	struct __attribute__((packed)) { u8 _pad_unkAD[0xAD]; byte unkAD; };
	struct __attribute__((packed)) { u8 _pad_disable[0xAE]; byte disable; };
	struct __attribute__((packed)) { u8 _pad_state[0xAF]; char state; };
	struct __attribute__((packed)) { u8 _pad_unkB0[0xB0]; byte unkB0; };
	struct __attribute__((packed)) { u8 _pad_unkB2[0xB2]; short unkB2; };
	struct __attribute__((packed)) { u8 _pad_unkB4[0xB4]; byte unkB4; };
	struct __attribute__((packed)) { u8 _pad_unkB5[0xB5]; byte unkB5; };
	struct __attribute__((packed)) { u8 _pad_flagsB6[0xB6]; HitboxFlags62 flagsB6; };
} HitState;
//include/sfa/xml/Game/GameObject/Hitbox.xml
typedef union ObjHitsEntry {
	short animId;
} ObjHitsEntry;
