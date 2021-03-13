//include/sfa/xml/Game/GameObject/ObjState.xml
typedef enum { //type:u8
	ObjState_CFLevelControl_flags0C_Lever2Activated = 0x20,
	ObjState_CFLevelControl_flags0C_DidntSetupObjs = 0x8,
	ObjState_CFLevelControl_flags0C_Lever1Activated = 0x10,
} ObjState_CFLevelControl_flags0C;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef enum { //type:u8
	ObjState_MagicPlant_Step_DropGem = 0x4,
	ObjState_MagicPlant_Step_Dead = 0x3,
	ObjState_MagicPlant_Step_Idle = 0x0,
	ObjState_MagicPlant_Step_FadeOut = 0x2,
	ObjState_MagicPlant_Step_Dying = 0x1,
} ObjState_MagicPlant_Step;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef enum { //type:u8
	ObjState_SeqObj2_flags_Used = 0x1,
	ObjState_SeqObj2_flags_Running = 0x2,
} ObjState_SeqObj2_flags;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef enum { //type:u8
	ObjState_SnowBike_Flags428_Riding = 0x2,
	ObjState_SnowBike_Flags428_PhysicsRelated80 = 0x80,
} ObjState_SnowBike_Flags428;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union BombPlantStateStruct {
	struct __attribute__((packed)) { u8 _pad_animScale[0x4]; float animScale; };
	struct __attribute__((packed)) { u8 _pad_flags[0x8]; byte flags; };
	struct __attribute__((packed)) { u8 _pad_unk09[0x9]; byte unk09; };
	struct __attribute__((packed)) { u8 _pad_unk0A[0xA]; byte unk0A; };
	struct __attribute__((packed)) { u8 _pad_unk0B[0xB]; byte unk0B; };
} BombPlantStateStruct;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjAnimStateCommon {
	byte seqFlags00;
	struct __attribute__((packed)) { u8 _pad_seqbit80[0x80]; GameBit16 seqbit80; };
} ObjAnimStateCommon;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObJSeqCurveIdxs {
	short rot9E_Z;
	struct __attribute__((packed)) { u8 _pad_rot9E_X[0x2]; short rot9E_X; };
	struct __attribute__((packed)) { u8 _pad_rot9E_Y[0x4]; short rot9E_Y; };
	struct __attribute__((packed)) { u8 _pad_opacity[0x6]; short opacity; };
	struct __attribute__((packed)) { u8 _pad_timeOfDay[0x8]; short timeOfDay; };
	struct __attribute__((packed)) { u8 _pad_scale[0xA]; short scale; };
	struct __attribute__((packed)) { u8 _pad_rotZ[0xC]; short rotZ; };
	struct __attribute__((packed)) { u8 _pad_rotX[0xE]; short rotX; };
	struct __attribute__((packed)) { u8 _pad_rotY[0x10]; short rotY; };
	struct __attribute__((packed)) { u8 _pad_animTime[0x12]; short animTime; };
	struct __attribute__((packed)) { u8 _pad_pointSound[0x14]; short pointSound; };
	struct __attribute__((packed)) { u8 _pad_posZ[0x16]; short posZ; };
	struct __attribute__((packed)) { u8 _pad_posY[0x18]; short posY; };
	struct __attribute__((packed)) { u8 _pad_posX[0x1A]; short posX; };
	struct __attribute__((packed)) { u8 _pad_unk0E[0x1C]; short unk0E; };
	struct __attribute__((packed)) { u8 _pad_eyeX[0x1E]; short eyeX; };
	struct __attribute__((packed)) { u8 _pad_eyeY[0x20]; short eyeY; };
	struct __attribute__((packed)) { u8 _pad_rot9E_X2[0x22]; short rot9E_X2; };
	struct __attribute__((packed)) { u8 _pad_soundAttr12[0x24]; short soundAttr12; };
} ObJSeqCurveIdxs;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjSeqCurvePositions {
	short rot9E_Z;
	struct __attribute__((packed)) { u8 _pad_rot9E_X[0x2]; short rot9E_X; };
	struct __attribute__((packed)) { u8 _pad_rot9E_Y[0x4]; short rot9E_Y; };
	struct __attribute__((packed)) { u8 _pad_opacity[0x6]; short opacity; };
	struct __attribute__((packed)) { u8 _pad_timeOfDay[0x8]; short timeOfDay; };
	struct __attribute__((packed)) { u8 _pad_scale[0xA]; short scale; };
	struct __attribute__((packed)) { u8 _pad_rotZ[0xC]; short rotZ; };
	struct __attribute__((packed)) { u8 _pad_rotX[0xE]; short rotX; };
	struct __attribute__((packed)) { u8 _pad_rotY[0x10]; short rotY; };
	struct __attribute__((packed)) { u8 _pad_animTime[0x12]; short animTime; };
	struct __attribute__((packed)) { u8 _pad_animIdx[0x14]; short animIdx; };
	struct __attribute__((packed)) { u8 _pad_posZ[0x16]; short posZ; };
	struct __attribute__((packed)) { u8 _pad_posY[0x18]; short posY; };
	struct __attribute__((packed)) { u8 _pad_posX[0x1A]; short posX; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; short unk1C; };
	struct __attribute__((packed)) { u8 _pad_eyeX[0x1E]; short eyeX; };
	struct __attribute__((packed)) { u8 _pad_eyeY[0x20]; short eyeY; };
	struct __attribute__((packed)) { u8 _pad_rot9E_X2[0x22]; short rot9E_X2; };
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; short unk24; };
} ObjSeqCurvePositions;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_AppleOnTree {
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; float unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; float unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; float unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; float unk14; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; float unk18; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; float unk1C; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; float unk20; };
	struct __attribute__((packed)) { u8 _pad_unk28[0x28]; float unk28; };
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; float unk2C; };
	struct __attribute__((packed)) { u8 _pad_unk30[0x30]; float unk30; };
	struct __attribute__((packed)) { u8 _pad_unk34[0x34]; float unk34; };
	struct __attribute__((packed)) { u8 _pad_healthValue[0x38]; ushort healthValue; };
	struct __attribute__((packed)) { u8 _pad_unk3C[0x3C]; float unk3C; };
	struct __attribute__((packed)) { u8 _pad_unk40[0x40]; float unk40; };
	struct __attribute__((packed)) { u8 _pad_unk44[0x44]; float unk44; };
	struct __attribute__((packed)) { u8 _pad_unk48[0x48]; short unk48; };
	struct __attribute__((packed)) { u8 _pad_unk4A[0x4A]; short unk4A; };
	struct __attribute__((packed)) { u8 _pad_unk4C[0x4C]; short unk4C; };
	struct __attribute__((packed)) { u8 _pad_unk50[0x50]; float unk50; };
	struct __attribute__((packed)) { u8 _pad_unk5A[0x5A]; byte unk5A; };
} ObjState_AppleOnTree;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_Arwing {
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; ObjInstance* unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; ObjInstance* unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; ObjInstance* unk0C; };
	struct __attribute__((packed)) { u8 _pad_obj10[0x10]; ObjInstance* obj10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; vec3f unk14; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; undefined4 unk20; };
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; float unk24; };
	struct __attribute__((packed)) { u8 _pad_unk28[0x28]; undefined4 unk28; };
	struct __attribute__((packed)) { u8 _pad_unk38[0x38]; float unk38; };
	struct __attribute__((packed)) { u8 _pad_unk50[0x50]; float unk50; };
	struct __attribute__((packed)) { u8 _pad_unk54[0x54]; undefined4 unk54; };
	struct __attribute__((packed)) { u8 _pad_unk58[0x58]; undefined4 unk58; };
	struct __attribute__((packed)) { u8 _pad_unk5C[0x5C]; float unk5C; };
	struct __attribute__((packed)) { u8 _pad_unk60[0x60]; undefined4 unk60; };
	struct __attribute__((packed)) { u8 _pad_unk64[0x64]; undefined4 unk64; };
	struct __attribute__((packed)) { u8 _pad_unk68[0x68]; undefined4 unk68; };
	struct __attribute__((packed)) { u8 _pad_unk6C[0x6C]; undefined4 unk6C; };
	struct __attribute__((packed)) { u8 _pad_unk78[0x78]; undefined4 unk78; };
	struct __attribute__((packed)) { u8 _pad_unk84[0x84]; undefined4 unk84; };
	struct __attribute__((packed)) { u8 _pad_unk88[0x88]; undefined4 unk88; };
	struct __attribute__((packed)) { u8 _pad_unk8C[0x8C]; undefined4 unk8C; };
	struct __attribute__((packed)) { u8 _pad_unk90[0x90]; undefined4 unk90; };
	struct __attribute__((packed)) { u8 _pad_unk94[0x94]; undefined4 unk94; };
	struct __attribute__((packed)) { u8 _pad_unk98[0x98]; undefined4 unk98; };
	struct __attribute__((packed)) { u8 _pad_timer9C[0x9C]; float timer9C; };
	struct __attribute__((packed)) { u8 _pad_unkA0[0xA0]; float unkA0; };
	struct __attribute__((packed)) { u8 _pad_unkA4[0xA4]; undefined4 unkA4; };
	struct __attribute__((packed)) { u8 _pad_unkA8[0xA8]; float unkA8; };
	struct __attribute__((packed)) { u8 _pad_unkAC[0xAC]; float unkAC; };
	struct __attribute__((packed)) { u8 _pad_unkB0[0xB0]; float unkB0; };
	struct __attribute__((packed)) { u8 _pad_timerB4[0xB4]; float timerB4; };
	struct __attribute__((packed)) { u8 _pad_unkB8[0xB8]; float unkB8; };
	struct __attribute__((packed)) { u8 _pad_unkBC[0xBC]; float unkBC; };
	struct __attribute__((packed)) { u8 _pad_scoreRelated339[0x339]; char scoreRelated339; };
	struct __attribute__((packed)) { u8 _pad_unk348[0x348]; undefined4 unk348; };
	struct __attribute__((packed)) { u8 _pad_unk34C[0x34C]; undefined4 unk34C; };
	struct __attribute__((packed)) { u8 _pad_unk35C[0x35C]; undefined4 unk35C; };
	struct __attribute__((packed)) { u8 _pad_unk360[0x360]; undefined4 unk360; };
	struct __attribute__((packed)) { u8 _pad_unk370[0x370]; undefined4 unk370; };
	struct __attribute__((packed)) { u8 _pad_unk374[0x374]; undefined4 unk374; };
	struct __attribute__((packed)) { u8 _pad_unk384[0x384]; undefined4 unk384; };
	struct __attribute__((packed)) { u8 _pad_unk388[0x388]; undefined4 unk388; };
	struct __attribute__((packed)) { u8 _pad_unk390[0x390]; undefined4 unk390; };
	struct __attribute__((packed)) { u8 _pad_unk394[0x394]; undefined4 unk394; };
	struct __attribute__((packed)) { u8 _pad_unk39C[0x39C]; undefined4 unk39C; };
	struct __attribute__((packed)) { u8 _pad_unk3A4[0x3A4]; undefined4 unk3A4; };
	struct __attribute__((packed)) { u8 _pad_unk3AC[0x3AC]; undefined4 unk3AC; };
	struct __attribute__((packed)) { u8 _pad_unk3B0[0x3B0]; undefined4 unk3B0; };
	struct __attribute__((packed)) { u8 _pad_unk3B4[0x3B4]; float unk3B4; };
	struct __attribute__((packed)) { u8 _pad_unk3B8[0x3B8]; float unk3B8; };
	struct __attribute__((packed)) { u8 _pad_unk3BC[0x3BC]; undefined4 unk3BC; };
	struct __attribute__((packed)) { u8 _pad_unk3C4[0x3C4]; float unk3C4; };
	struct __attribute__((packed)) { u8 _pad_unk3C8[0x3C8]; undefined4 unk3C8; };
	struct __attribute__((packed)) { u8 _pad_unk3D0[0x3D0]; undefined4 unk3D0; };
	struct __attribute__((packed)) { u8 _pad_unk3D4[0x3D4]; undefined4 unk3D4; };
	struct __attribute__((packed)) { u8 _pad_unk3E0[0x3E0]; undefined4 unk3E0; };
	struct __attribute__((packed)) { u8 _pad_unk3F4[0x3F4]; ushort unk3F4; };
	struct __attribute__((packed)) { u8 _pad_unk3F6[0x3F6]; ushort unk3F6; };
	struct __attribute__((packed)) { u8 _pad_unk3FA[0x3FA]; byte unk3FA; };
	struct __attribute__((packed)) { u8 _pad_unk404[0x404]; byte unk404; };
	struct __attribute__((packed)) { u8 _pad_unk40C[0x40C]; undefined2 unk40C; };
	struct __attribute__((packed)) { u8 _pad_laserVal40E[0x40E]; undefined2 laserVal40E; };
	struct __attribute__((packed)) { u8 _pad_laserVal410[0x410]; undefined4 laserVal410; };
	struct __attribute__((packed)) { u8 _pad_unk418[0x418]; ObjInstance* unk418; };
	struct __attribute__((packed)) { u8 _pad_unk41C[0x41C]; ObjInstance* unk41C; };
	struct __attribute__((packed)) { u8 _pad_curBomb[0x438]; ObjInstance* curBomb; };
	struct __attribute__((packed)) { u8 _pad_unk43D[0x43D]; byte unk43D; };
	struct __attribute__((packed)) { u8 _pad_unk444[0x444]; undefined2 unk444; };
	struct __attribute__((packed)) { u8 _pad_unk446[0x446]; ushort unk446; };
	struct __attribute__((packed)) { u8 _pad_unk448[0x448]; float unk448; };
	struct __attribute__((packed)) { u8 _pad_nBombs[0x44C]; byte nBombs; };
	struct __attribute__((packed)) { u8 _pad_maxBombs[0x44D]; byte maxBombs; };
	struct __attribute__((packed)) { u8 _pad_unk44E[0x44E]; ushort unk44E; };
	struct __attribute__((packed)) { u8 _pad_unk450[0x450]; ObjLight* unk450; };
	struct __attribute__((packed)) { u8 _pad_unk454[0x454]; vec3s* unk454; };
	struct __attribute__((packed)) { u8 _pad_unk458[0x458]; vec3s* unk458; };
	struct __attribute__((packed)) { u8 _pad_unk45C[0x45C]; vec3s* unk45C; };
	struct __attribute__((packed)) { u8 _pad_unk460[0x460]; vec3s* unk460; };
	struct __attribute__((packed)) { u8 _pad_unk464[0x464]; undefined4 unk464; };
	struct __attribute__((packed)) { u8 _pad_curHealth[0x468]; byte curHealth; };
	struct __attribute__((packed)) { u8 _pad_maxHealth[0x469]; s8 maxHealth; };
	struct __attribute__((packed)) { u8 _pad_nGoldRings[0x470]; byte nGoldRings; };
	struct __attribute__((packed)) { u8 _pad_nGoldRingsNeeded[0x471]; byte nGoldRingsNeeded; };
	struct __attribute__((packed)) { u8 _pad_unk472[0x472]; byte unk472; };
	struct __attribute__((packed)) { u8 _pad_unk473[0x473]; byte unk473; };
	struct __attribute__((packed)) { u8 _pad_unk474[0x474]; byte unk474; };
	struct __attribute__((packed)) { u8 _pad_unk475[0x475]; byte unk475; };
	struct __attribute__((packed)) { u8 _pad_unk476[0x476]; byte unk476; };
	struct __attribute__((packed)) { u8 _pad_flags477[0x477]; byte flags477; };
	struct __attribute__((packed)) { u8 _pad_unk478[0x478]; byte unk478; };
	struct __attribute__((packed)) { u8 _pad_destMapIdx[0x47B]; byte destMapIdx; };
	struct __attribute__((packed)) { u8 _pad_score[0x47C]; ushort score; };
	struct __attribute__((packed)) { u8 _pad_levelNo[0x47E]; byte levelNo; };
	struct __attribute__((packed)) { u8 _pad_unk47F[0x47F]; byte unk47F; };
	struct __attribute__((packed)) { u8 _pad_unk480[0x480]; char unk480; };
	struct __attribute__((packed)) { u8 _pad_pos484[0x484]; vec3f pos484; };
	struct __attribute__((packed)) { u8 _pad_rot490[0x490]; vec3s rot490; };
} ObjState_Arwing;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_ArwingAndrossStuff {
	byte type;
	struct __attribute__((packed)) { u8 _pad_timer4[0x4]; float timer4; };
	struct __attribute__((packed)) { u8 _pad_timer10[0x10]; float timer10; };
	struct __attribute__((packed)) { u8 _pad_light[0x14]; ObjLight* light; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; byte unk18; };
	struct __attribute__((packed)) { u8 _pad_unk1A[0x1A]; short unk1A; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; short unk1C; };
} ObjState_ArwingAndrossStuff;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_ARWLevelCon {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; float unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; float unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; undefined2 unk14; };
	struct __attribute__((packed)) { u8 _pad_unk16[0x16]; undefined2 unk16; };
	struct __attribute__((packed)) { u8 _pad_didInitFx[0x18]; char didInitFx; };
	struct __attribute__((packed)) { u8 _pad_didPlayStream[0x19]; char didPlayStream; };
	struct __attribute__((packed)) { u8 _pad_isFinished[0x1A]; char isFinished; };
	struct __attribute__((packed)) { u8 _pad_isAndross[0x1B]; byte isAndross; };
	struct __attribute__((packed)) { u8 _pad_streamId[0x1C]; undefined4 streamId; };
	struct __attribute__((packed)) { u8 _pad_seqObjDef[0x20]; ObjDefEnum seqObjDef; };
} ObjState_ARWLevelCon;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_Baddie {
	void* unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; void* unk04; };
	struct __attribute__((packed)) { u8 _pad_target[0x29C]; ObjInstance* target; };
	struct __attribute__((packed)) { u8 _pad_targetAngleHi[0x2A0]; ushort targetAngleHi; };
	struct __attribute__((packed)) { u8 _pad_targetAngleLo[0x2A2]; short targetAngleLo; };
	struct __attribute__((packed)) { u8 _pad_targetDistXZ[0x2A4]; ushort targetDistXZ; };
	struct __attribute__((packed)) { u8 _pad_targetDistY[0x2A6]; short targetDistY; };
	struct __attribute__((packed)) { u8 _pad_unk2A8[0x2A8]; void* unk2A8; };
	struct __attribute__((packed)) { u8 _pad_unk2AC[0x2AC]; void* unk2AC; };
	struct __attribute__((packed)) { u8 _pad_health[0x2B0]; ushort health; };
	struct __attribute__((packed)) { u8 _pad_unk2B2[0x2B2]; ushort unk2B2; };
	struct __attribute__((packed)) { u8 _pad_loadedWeaponId[0x2B4]; ObjDefEnum loadedWeaponId; };
	struct __attribute__((packed)) { u8 _pad_weaponId[0x2B6]; ObjDefEnum weaponId; };
	struct __attribute__((packed)) { u8 _pad_timer02D0[0x2D0]; float timer02D0; };
	struct __attribute__((packed)) { u8 _pad_frozenTimer[0x2D4]; float frozenTimer; };
	struct __attribute__((packed)) { u8 _pad_damage[0x2D8]; void* damage; };
	struct __attribute__((packed)) { u8 _pad_flags02DC[0x2DC]; BaddieFlags02DC flags02DC; };
	struct __attribute__((packed)) { u8 _pad_prevFlags[0x2E0]; BaddieFlags02DC prevFlags; };
	struct __attribute__((packed)) { u8 _pad_unk2E4[0x2E4]; int unk2E4; };
	struct __attribute__((packed)) { u8 _pad_flags02E8[0x2E8]; void* flags02E8; };
	struct __attribute__((packed)) { u8 _pad_unk2EC[0x2EC]; undefined2 unk2EC; };
	struct __attribute__((packed)) { u8 _pad_unk2F1[0x2F1]; byte unk2F1; };
	struct __attribute__((packed)) { u8 _pad_unk2F2[0x2F2]; byte unk2F2; };
	struct __attribute__((packed)) { u8 _pad_unk2F5[0x2F5]; byte unk2F5; };
	struct __attribute__((packed)) { u8 _pad_unk2F6[0x2F6]; byte unk2F6; };
	struct __attribute__((packed)) { u8 _pad_unk2F8[0x2F8]; undefined2 unk2F8; };
	struct __attribute__((packed)) { u8 _pad_unk2FC[0x2FC]; void* unk2FC; };
	struct __attribute__((packed)) { u8 _pad_unk300[0x300]; void* unk300; };
	struct __attribute__((packed)) { u8 _pad_unk304[0x304]; void* unk304; };
	struct __attribute__((packed)) { u8 _pad_unk308[0x308]; void* unk308; };
	struct __attribute__((packed)) { u8 _pad_unk30C[0x30C]; void* unk30C; };
	struct __attribute__((packed)) { u8 _pad_unk310[0x310]; void* unk310; };
	struct __attribute__((packed)) { u8 _pad_unk314[0x314]; float unk314; };
	struct __attribute__((packed)) { u8 _pad_unk318[0x318]; float unk318; };
	struct __attribute__((packed)) { u8 _pad_unk31C[0x31C]; float unk31C; };
	struct __attribute__((packed)) { u8 _pad_unk320[0x320]; byte unk320; };
	struct __attribute__((packed)) { u8 _pad_unk321[0x321]; byte unk321; };
	struct __attribute__((packed)) { u8 _pad_unk322[0x322]; byte unk322; };
	struct __attribute__((packed)) { u8 _pad_unk323[0x323]; byte unk323; };
	struct __attribute__((packed)) { u8 _pad_unk324[0x324]; void* unk324; };
	struct __attribute__((packed)) { u8 _pad_unk328[0x328]; void* unk328; };
	struct __attribute__((packed)) { u8 _pad_ypos032C[0x32C]; void* ypos032C; };
	struct __attribute__((packed)) { u8 _pad_unk330[0x330]; void* unk330; };
	struct __attribute__((packed)) { u8 _pad_unk334[0x334]; void* unk334; };
	struct __attribute__((packed)) { u8 _pad_unk338[0x338]; ushort unk338; };
	struct __attribute__((packed)) { u8 _pad_unk33A[0x33A]; byte unk33A; };
	struct __attribute__((packed)) { u8 _pad_isFireBat[0x33B]; char isFireBat; };
	struct __attribute__((packed)) { u8 _pad_unk33C[0x33C]; byte unk33C; };
	struct __attribute__((packed)) { u8 _pad_unk33D[0x33D]; byte unk33D; };
	struct __attribute__((packed)) { u8 _pad_light[0x368]; ObjLight* light; };
	struct __attribute__((packed)) { u8 _pad_unk36C[0x36C]; ModelStruct55* unk36C; };
} ObjState_Baddie;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_BombPlant {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; float unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_scale[0xC]; float scale; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; float unk10; };
	struct __attribute__((packed)) { u8 _pad_state[0x14]; byte state; };
	struct __attribute__((packed)) { u8 _pad_flags15[0x15]; byte flags15; };
} ObjState_BombPlant;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_Carryable {
	short unk00;
	struct __attribute__((packed)) { u8 _pad_unk02[0x2]; short unk02; };
	struct __attribute__((packed)) { u8 _pad_unk05[0x5]; char unk05; };
	struct __attribute__((packed)) { u8 _pad_unk06[0x6]; char unk06; };
	struct __attribute__((packed)) { u8 _pad_flags[0x7]; byte flags; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; byte unk08; };
	struct __attribute__((packed)) { u8 _pad_unk09[0x9]; byte unk09; };
} ObjState_Carryable;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_CFLevelControl {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_flags08[0x8]; uint flags08; };
	struct __attribute__((packed)) { u8 _pad_flags0C[0xC]; ObjState_CFLevelControl_flags0C flags0C; };
	struct __attribute__((packed)) { u8 _pad_camMode[0xD]; byte camMode; };
} ObjState_CFLevelControl;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_CNTcounter {
	uint count;
} ObjState_CNTcounter;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_DeathSeq {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_pos[0x4]; vec3f pos; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; float unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; float unk14; };
	struct __attribute__((packed)) { u8 _pad_rotX[0x18]; float rotX; };
	struct __attribute__((packed)) { u8 _pad_rotY[0x1C]; float rotY; };
	struct __attribute__((packed)) { u8 _pad_flags20[0x20]; byte flags20; };
} ObjState_DeathSeq;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_dll224 {
	short stoneUsed;
	struct __attribute__((packed)) { u8 _pad_enable[0x2]; short enable; };
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; char unk04; };
} ObjState_dll224;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_FuelCell {
	undefined2 unk00;
	struct __attribute__((packed)) { u8 _pad_flags5C[0x5C]; byte flags5C; };
} ObjState_FuelCell;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_GunPowderBarrel {
	ObjState_Carryable unk00;
	struct __attribute__((packed)) { u8 _pad_flags[0xD]; byte flags; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; ObjInstance* unk10; };
	struct __attribute__((packed)) { u8 _pad_unk15[0x15]; char unk15; };
	struct __attribute__((packed)) { u8 _pad_unk16[0x16]; char unk16; };
	struct __attribute__((packed)) { u8 _pad_unk17[0x17]; byte unk17; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; float unk18; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; vec3f unk20; };
	struct __attribute__((packed)) { u8 _pad_unk34[0x34]; float unk34; };
	struct __attribute__((packed)) { u8 _pad_unk48[0x48]; byte unk48; };
	struct __attribute__((packed)) { u8 _pad_unk49[0x49]; byte unk49; };
	struct __attribute__((packed)) { u8 _pad_unk4A[0x4A]; byte unk4A; };
	struct __attribute__((packed)) { u8 _pad_timer54[0x54]; float timer54; };
} ObjState_GunPowderBarrel;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_InfoPoint {
	gametextStruct* text;
	struct __attribute__((packed)) { u8 _pad_phrases[0x4]; char* phrases; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; undefined* unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; undefined4 unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; byte unk10; };
	struct __attribute__((packed)) { u8 _pad_unk16[0x16]; undefined2 unk16; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; undefined4 unk18; };
} ObjState_InfoPoint;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_Laser {
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_light[0x14]; ObjLight* light; };
} ObjState_Laser;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_LevelName {
	gametextStruct* text;
	struct __attribute__((packed)) { u8 _pad_phrases[0x4]; char* phrases; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; undefined4 unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; byte unk0C; };
	struct __attribute__((packed)) { u8 _pad_bit[0xE]; ObjDefEnum bit; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; undefined2 unk10; };
	struct __attribute__((packed)) { u8 _pad_unk12[0x12]; undefined2 unk12; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; byte unk14; };
} ObjState_LevelName;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_LinkA_levelcontrol {
	struct __attribute__((packed)) { u8 _pad_actions[0x81]; byte actions; };
	struct __attribute__((packed)) { u8 _pad_nActions[0x8B]; byte nActions; };
} ObjState_LinkA_levelcontrol;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_MagicCaveTop {
	byte action;
	struct __attribute__((packed)) { u8 _pad_flags01[0x1]; byte flags01; };
	struct __attribute__((packed)) { u8 _pad_unk02[0x2]; byte unk02; };
	struct __attribute__((packed)) { u8 _pad_warpTimer04[0x4]; float warpTimer04; };
	struct __attribute__((packed)) { u8 _pad_timer08[0x8]; float timer08; };
} ObjState_MagicCaveTop;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_MagicDust {
	struct __attribute__((packed)) { u8 _pad_unk6C[0x6C]; float unk6C; };
	struct __attribute__((packed)) { u8 _pad_unk25B[0x25B]; byte unk25B; };
	struct __attribute__((packed)) { u8 _pad_unk268[0x268]; float unk268; };
	struct __attribute__((packed)) { u8 _pad_vanishTimer[0x26C]; float vanishTimer; };
	struct __attribute__((packed)) { u8 _pad_unk270[0x270]; undefined2 unk270; };
	struct __attribute__((packed)) { u8 _pad_unk272[0x272]; undefined2 unk272; };
	struct __attribute__((packed)) { u8 _pad_unk274[0x274]; SoundId unk274; };
	struct __attribute__((packed)) { u8 _pad_unk276[0x276]; undefined2 unk276; };
	struct __attribute__((packed)) { u8 _pad_unk278[0x278]; ushort unk278; };
	struct __attribute__((packed)) { u8 _pad_flags27A[0x27A]; byte flags27A; };
	struct __attribute__((packed)) { u8 _pad_unk27B[0x27B]; byte unk27B; };
	struct __attribute__((packed)) { u8 _pad_unk27C[0x27C]; byte unk27C; };
	struct __attribute__((packed)) { u8 _pad_unk280[0x280]; short unk280; };
} ObjState_MagicDust;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_MagicPlant {
	ObjInstance* gem;
	struct __attribute__((packed)) { u8 _pad_growTime[0x4]; float growTime; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; ObjInstance* unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; short unk0C; };
	struct __attribute__((packed)) { u8 _pad_step[0xF]; ObjState_MagicPlant_Step step; };
} ObjState_MagicPlant;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_Pushable {
	struct __attribute__((packed)) { u8 _pad_bitAC[0xAC]; GameBit16 bitAC; };
	struct __attribute__((packed)) { u8 _pad_lastMsgF0003from[0xB8]; ObjInstance* lastMsgF0003from; };
	struct __attribute__((packed)) { u8 _pad_unkBC[0xBC]; ObjInstance* unkBC; };
	struct __attribute__((packed)) { u8 _pad_unkCC[0xCC]; float unkCC; };
	struct __attribute__((packed)) { u8 _pad_unkD0[0xD0]; float unkD0; };
	struct __attribute__((packed)) { u8 _pad_unkD4[0xD4]; float unkD4; };
	struct __attribute__((packed)) { u8 _pad_unkD8[0xD8]; float unkD8; };
	struct __attribute__((packed)) { u8 _pad_unkDC[0xDC]; float unkDC; };
	struct __attribute__((packed)) { u8 _pad_unkE0[0xE0]; float unkE0; };
	struct __attribute__((packed)) { u8 _pad_unkE4[0xE4]; float unkE4; };
	struct __attribute__((packed)) { u8 _pad_unkE8[0xE8]; float unkE8; };
	struct __attribute__((packed)) { u8 _pad_unkEC[0xEC]; float unkEC; };
	struct __attribute__((packed)) { u8 _pad_unkF0[0xF0]; float unkF0; };
	struct __attribute__((packed)) { u8 _pad_unkF4[0xF4]; float unkF4; };
	struct __attribute__((packed)) { u8 _pad_unkF8[0xF8]; float unkF8; };
	struct __attribute__((packed)) { u8 _pad_flags100[0x100]; ushort flags100; };
	struct __attribute__((packed)) { u8 _pad_unk114[0x114]; byte unk114; };
	struct __attribute__((packed)) { u8 _pad_unk145[0x145]; char unk145; };
	struct __attribute__((packed)) { u8 _pad_unk146[0x146]; byte unk146; };
} ObjState_Pushable;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_Ring {
	byte unk00;
	struct __attribute__((packed)) { u8 _pad_type[0x1]; byte type; };
	struct __attribute__((packed)) { u8 _pad_unk02[0x2]; ushort unk02; };
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; float unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; float unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; float unk10; };
	struct __attribute__((packed)) { u8 _pad_flags14[0x14]; byte flags14; };
	struct __attribute__((packed)) { u8 _pad_unk15[0x15]; byte unk15; };
	struct __attribute__((packed)) { u8 _pad_timer18[0x18]; float timer18; };
	struct __attribute__((packed)) { u8 _pad_light[0x20]; ObjLight* light; };
} ObjState_Ring;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_SB_Galleon {
	vec3f unk00;
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; float unk1C; };
	struct __attribute__((packed)) { u8 _pad_rot[0x20]; vec3s rot; };
	struct __attribute__((packed)) { u8 _pad_frameCount26[0x26]; undefined2 frameCount26; };
	struct __attribute__((packed)) { u8 _pad_unk28[0x28]; byte unk28; };
	struct __attribute__((packed)) { u8 _pad_action29[0x29]; byte action29; };
	struct __attribute__((packed)) { u8 _pad_unk2A[0x2A]; byte unk2A; };
	struct __attribute__((packed)) { u8 _pad_unk2B[0x2B]; byte unk2B; };
	struct __attribute__((packed)) { u8 _pad_pos[0x2C]; vec3f pos; };
	struct __attribute__((packed)) { u8 _pad_unk38[0x38]; float unk38; };
	struct __attribute__((packed)) { u8 _pad_unk3C[0x3C]; float unk3C; };
	struct __attribute__((packed)) { u8 _pad_unk40[0x40]; float unk40; };
	struct __attribute__((packed)) { u8 _pad_unk44[0x44]; float unk44; };
	struct __attribute__((packed)) { u8 _pad_pShipHead[0x4C]; ObjInstance* pShipHead; };
	struct __attribute__((packed)) { u8 _pad_unk50[0x50]; float unk50; };
	struct __attribute__((packed)) { u8 _pad_unk58[0x58]; float unk58; };
	struct __attribute__((packed)) { u8 _pad_unk5C[0x5C]; vec3f unk5C; };
	struct __attribute__((packed)) { u8 _pad_frameCount6C[0x6C]; short frameCount6C; };
	struct __attribute__((packed)) { u8 _pad_frameCount6E[0x6E]; undefined2 frameCount6E; };
	struct __attribute__((packed)) { u8 _pad_unk70[0x70]; byte unk70; };
	struct __attribute__((packed)) { u8 _pad_mapId[0x72]; short mapId; };
	struct __attribute__((packed)) { u8 _pad_showIntroTextTimer[0x74]; float showIntroTextTimer; };
	struct __attribute__((packed)) { u8 _pad_unk78[0x78]; byte unk78; };
	struct __attribute__((packed)) { u8 _pad_unk79[0x79]; byte unk79; };
	struct __attribute__((packed)) { u8 _pad_unk7A[0x7A]; byte unk7A; };
	struct __attribute__((packed)) { u8 _pad_unk7B[0x7B]; byte unk7B; };
	struct __attribute__((packed)) { u8 _pad_unk7C[0x7C]; byte unk7C; };
	struct __attribute__((packed)) { u8 _pad_unk80[0x80]; byte unk80; };
	struct __attribute__((packed)) { u8 _pad_unk82[0x82]; undefined2 unk82; };
	struct __attribute__((packed)) { u8 _pad_unk84[0x84]; byte unk84; };
	struct __attribute__((packed)) { u8 _pad_unk85[0x85]; byte unk85; };
	struct __attribute__((packed)) { u8 _pad_timer88[0x88]; float timer88; };
	struct __attribute__((packed)) { u8 _pad_timer8C[0x8C]; float timer8C; };
	struct __attribute__((packed)) { u8 _pad_timer90[0x90]; float timer90; };
	struct __attribute__((packed)) { u8 _pad_timer94[0x94]; float timer94; };
	struct __attribute__((packed)) { u8 _pad_unk98[0x98]; int unk98; };
	struct __attribute__((packed)) { u8 _pad_songId[0x9C]; int songId; };
	struct __attribute__((packed)) { u8 _pad_unkA0[0xA0]; byte unkA0; };
	struct __attribute__((packed)) { u8 _pad_unkA1[0xA1]; byte unkA1; };
	struct __attribute__((packed)) { u8 _pad_unkA5[0xA5]; byte unkA5; };
	struct __attribute__((packed)) { u8 _pad_unkA6[0xA6]; byte unkA6; };
	struct __attribute__((packed)) { u8 _pad_unkA7[0xA7]; byte unkA7; };
	struct __attribute__((packed)) { u8 _pad_unkA8[0xA8]; byte unkA8; };
	struct __attribute__((packed)) { u8 _pad_unkA9[0xA9]; byte unkA9; };
	struct __attribute__((packed)) { u8 _pad_unkAA[0xAA]; byte unkAA; };
	struct __attribute__((packed)) { u8 _pad_unkAB[0xAB]; byte unkAB; };
	struct __attribute__((packed)) { u8 _pad_timerAC[0xAC]; float timerAC; };
	struct __attribute__((packed)) { u8 _pad_unkB0[0xB0]; undefined4 unkB0; };
} ObjState_SB_Galleon;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_Scarab {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; float unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; float unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; ushort unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; ushort unk14; };
	struct __attribute__((packed)) { u8 _pad_unk16[0x16]; short unk16; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; short unk18; };
	struct __attribute__((packed)) { u8 _pad_unk1A[0x1A]; ushort unk1A; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; short unk1C; };
	struct __attribute__((packed)) { u8 _pad_unk1E[0x1E]; SoundId unk1E; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; short unk20; };
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; char unk24; };
	struct __attribute__((packed)) { u8 _pad_type[0x27]; byte type; };
	struct __attribute__((packed)) { u8 _pad_unk28[0x28]; byte unk28; };
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; float unk2C; };
	struct __attribute__((packed)) { u8 _pad_unk30[0x30]; float unk30; };
} ObjState_Scarab;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_SeqObj2 {
	ObjState_SeqObj2_flags flags;
} ObjState_SeqObj2;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_SeqPoint {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_gameBit04[0x4]; undefined2 gameBit04; };
	struct __attribute__((packed)) { u8 _pad_gameBit06[0x6]; undefined2 gameBit06; };
	struct __attribute__((packed)) { u8 _pad_seqIdx[0x8]; undefined2 seqIdx; };
	struct __attribute__((packed)) { u8 _pad_unk0D[0xD]; byte unk0D; };
	struct __attribute__((packed)) { u8 _pad_unk0E[0xE]; byte unk0E; };
} ObjState_SeqPoint;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_SH_thorntail {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; float unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; float unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; vec3f unk10; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; float unk1C; };
	struct __attribute__((packed)) { u8 _pad_unk5BC[0x5BC]; float unk5BC; };
	struct __attribute__((packed)) { u8 _pad_unk5F8[0x5F8]; int unk5F8; };
	struct __attribute__((packed)) { u8 _pad_unk5FC[0x5FC]; int unk5FC; };
	struct __attribute__((packed)) { u8 _pad_unk600[0x600]; byte unk600; };
	struct __attribute__((packed)) { u8 _pad_unk601[0x601]; byte unk601; };
	struct __attribute__((packed)) { u8 _pad_unk604[0x604]; int unk604; };
	struct __attribute__((packed)) { u8 _pad_unk608[0x608]; int unk608; };
	struct __attribute__((packed)) { u8 _pad_unk60C[0x60C]; undefined2 unk60C; };
	struct __attribute__((packed)) { u8 _pad_unk60E[0x60E]; undefined2 unk60E; };
	struct __attribute__((packed)) { u8 _pad_count610[0x610]; byte count610; };
	struct __attribute__((packed)) { u8 _pad_unk611[0x611]; byte unk611; };
	struct __attribute__((packed)) { u8 _pad_unk614[0x614]; float unk614; };
	struct __attribute__((packed)) { u8 _pad_unk618[0x618]; int unk618; };
	struct __attribute__((packed)) { u8 _pad_frameCount620[0x620]; int frameCount620; };
	struct __attribute__((packed)) { u8 _pad_animIdx[0x624]; char animIdx; };
	struct __attribute__((packed)) { u8 _pad_flags625[0x625]; byte flags625; };
	struct __attribute__((packed)) { u8 _pad_unk627[0x627]; byte unk627; };
	struct __attribute__((packed)) { u8 _pad_unk628[0x628]; float unk628; };
	struct __attribute__((packed)) { u8 _pad_seqIds[0x62C]; byte* seqIds; };
	struct __attribute__((packed)) { u8 _pad_unk630[0x630]; float unk630; };
	struct __attribute__((packed)) { u8 _pad_unk638[0x638]; float unk638; };
	struct __attribute__((packed)) { u8 _pad_xRot63C[0x63C]; short xRot63C; };
	struct __attribute__((packed)) { u8 _pad_unk63F[0x63F]; byte unk63F; };
	struct __attribute__((packed)) { u8 _pad_unk640[0x640]; byte unk640; };
	struct __attribute__((packed)) { u8 _pad_unk7DC[0x7DC]; short unk7DC; };
	struct __attribute__((packed)) { u8 _pad_unk7DE[0x7DE]; short unk7DE; };
	struct __attribute__((packed)) { u8 _pad_unk8AC[0x8AC]; float unk8AC; };
	struct __attribute__((packed)) { u8 _pad_unk8B0[0x8B0]; EyeAnimStruct unk8B0; };
} ObjState_SH_thorntail;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_ShopKeeper {
	struct __attribute__((packed)) { u8 _pad_unk35C[0x35C]; ObjState_SH_thorntail unk35C; };
	struct __attribute__((packed)) { u8 _pad_eyeAnim[0x980]; EyeAnimStruct eyeAnim; };
	struct __attribute__((packed)) { u8 _pad_unk9B0[0x9B0]; undefined* unk9B0; };
	struct __attribute__((packed)) { u8 _pad_unk9B4[0x9B4]; ObjInstance* unk9B4; };
	struct __attribute__((packed)) { u8 _pad_unk9B8[0x9B8]; float unk9B8; };
	struct __attribute__((packed)) { u8 _pad_showMapNameTimer[0x9C4]; float showMapNameTimer; };
	struct __attribute__((packed)) { u8 _pad_playerMoney[0x9C8]; short playerMoney; };
	struct __attribute__((packed)) { u8 _pad_flags9D4[0x9D4]; byte flags9D4; };
	struct __attribute__((packed)) { u8 _pad_opacity[0x9D6]; byte opacity; };
} ObjState_ShopKeeper;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_SnowBike {
	vec3s lastCheckpointRot;
	struct __attribute__((packed)) { u8 _pad_lastCheckpointPos[0xC]; vec3f lastCheckpointPos; };
	struct __attribute__((packed)) { u8 _pad_checkpoint[0x28]; CheckpointStruct28 checkpoint; };
	struct __attribute__((packed)) { u8 _pad_unk5D[0x5D]; byte unk5D; };
	struct __attribute__((packed)) { u8 _pad_unk60[0x60]; GameBit16* unk60; };
	struct __attribute__((packed)) { u8 _pad_nextCheckpointAngle[0x68]; float nextCheckpointAngle; };
	struct __attribute__((packed)) { u8 _pad_mtx6C[0x6C]; Mtx44 mtx6C; };
	struct __attribute__((packed)) { u8 _pad_mtxAC[0xAC]; Mtx44 mtxAC; };
	struct __attribute__((packed)) { u8 _pad_mtxEC[0xEC]; Mtx44 mtxEC; };
	struct __attribute__((packed)) { u8 _pad_mtx12C[0x12C]; Mtx44 mtx12C; };
	struct __attribute__((packed)) { u8 _pad_unk3D3[0x3D3]; byte unk3D3; };
	struct __attribute__((packed)) { u8 _pad_unk3E0[0x3E0]; float unk3E0; };
	struct __attribute__((packed)) { u8 _pad_unk3E4[0x3E4]; float unk3E4; };
	struct __attribute__((packed)) { u8 _pad_unk3F4[0x3F4]; float unk3F4; };
	struct __attribute__((packed)) { u8 _pad_timer3F8[0x3F8]; float timer3F8; };
	struct __attribute__((packed)) { u8 _pad_xRot40C[0x40C]; short xRot40C; };
	struct __attribute__((packed)) { u8 _pad_xRot40E[0x40E]; short xRot40E; };
	struct __attribute__((packed)) { u8 _pad_unk410[0x410]; int unk410; };
	struct __attribute__((packed)) { u8 _pad_unk414[0x414]; undefined4 unk414; };
	struct __attribute__((packed)) { u8 _pad_unk41C[0x41C]; short unk41C; };
	struct __attribute__((packed)) { u8 _pad_unk41E[0x41E]; short unk41E; };
	struct __attribute__((packed)) { u8 _pad_unk420[0x420]; byte unk420; };
	struct __attribute__((packed)) { u8 _pad_unk421[0x421]; char unk421; };
	struct __attribute__((packed)) { u8 _pad_unk422[0x422]; byte unk422; };
	struct __attribute__((packed)) { u8 _pad_unk424[0x424]; float unk424; };
	struct __attribute__((packed)) { u8 _pad_flags428[0x428]; ObjState_SnowBike_Flags428 flags428; };
	struct __attribute__((packed)) { u8 _pad_unk42C[0x42C]; int unk42C; };
	struct __attribute__((packed)) { u8 _pad_unk430[0x430]; float unk430; };
	struct __attribute__((packed)) { u8 _pad_checkpointVar434[0x434]; byte checkpointVar434; };
	struct __attribute__((packed)) { u8 _pad_checkpointVar435[0x435]; byte checkpointVar435; };
	struct __attribute__((packed)) { u8 _pad_timer43C[0x43C]; float timer43C; };
	struct __attribute__((packed)) { u8 _pad_unk440[0x440]; SoundId unk440; };
	struct __attribute__((packed)) { u8 _pad_unk44A[0x44A]; GameBit16 unk44A; };
	struct __attribute__((packed)) { u8 _pad_stickX_44c[0x44C]; short stickX_44c; };
	struct __attribute__((packed)) { u8 _pad_buttonsPressed[0x450]; uint buttonsPressed; };
	struct __attribute__((packed)) { u8 _pad_buttonsPressedNotBusy[0x454]; uint buttonsPressedNotBusy; };
	struct __attribute__((packed)) { u8 _pad_buttonsHeld[0x458]; uint buttonsHeld; };
	struct __attribute__((packed)) { u8 _pad_stickX[0x45C]; float stickX; };
	struct __attribute__((packed)) { u8 _pad_stickY[0x460]; char stickY; };
	struct __attribute__((packed)) { u8 _pad_unk464[0x464]; Quaternion unk464; };
	struct __attribute__((packed)) { u8 _pad_unk47C[0x47C]; Quaternion unk47C; };
	struct __attribute__((packed)) { u8 _pad_unk494[0x494]; Quaternion unk494; };
	struct __attribute__((packed)) { u8 _pad_unk4B4[0x4B4]; byte unk4B4; };
	struct __attribute__((packed)) { u8 _pad_unk4B8[0x4B8]; float unk4B8; };
	struct __attribute__((packed)) { u8 _pad_unk4BC[0x4BC]; float unk4BC; };
	struct __attribute__((packed)) { u8 _pad_unk4C0[0x4C0]; float unk4C0; };
	struct __attribute__((packed)) { u8 _pad_unk4C4[0x4C4]; float unk4C4; };
	struct __attribute__((packed)) { u8 _pad_unk4C8[0x4C8]; byte unk4C8; };
	struct __attribute__((packed)) { u8 _pad_unk4CC[0x4CC]; short unk4CC; };
	struct __attribute__((packed)) { u8 _pad_flags4CE[0x4CE]; byte flags4CE; };
	struct __attribute__((packed)) { u8 _pad_unk510[0x510]; int unk510; };
	struct __attribute__((packed)) { u8 _pad_unk514[0x514]; int unk514; };
	struct __attribute__((packed)) { u8 _pad_unk518[0x518]; int unk518; };
	struct __attribute__((packed)) { u8 _pad_prevPos51C[0x51C]; vec3f prevPos51C; };
	struct __attribute__((packed)) { u8 _pad_unk52C[0x52C]; float unk52C; };
	struct __attribute__((packed)) { u8 _pad_turnMax530[0x530]; float turnMax530; };
	struct __attribute__((packed)) { u8 _pad_turnRate534[0x534]; float turnRate534; };
	struct __attribute__((packed)) { u8 _pad_unk538[0x538]; float unk538; };
	struct __attribute__((packed)) { u8 _pad_unk53C[0x53C]; float unk53C; };
	struct __attribute__((packed)) { u8 _pad_unk540[0x540]; float unk540; };
	struct __attribute__((packed)) { u8 _pad_unk544[0x544]; float unk544; };
	struct __attribute__((packed)) { u8 _pad_turnStrength548[0x548]; float turnStrength548; };
	struct __attribute__((packed)) { u8 _pad_topSpeed54C[0x54C]; float topSpeed54C; };
	struct __attribute__((packed)) { u8 _pad_turnVel558[0x558]; float turnVel558; };
	struct __attribute__((packed)) { u8 _pad_turnMax[0x56C]; float turnMax; };
	struct __attribute__((packed)) { u8 _pad_turnRate[0x574]; float turnRate; };
	struct __attribute__((packed)) { u8 _pad_turnVel[0x578]; float turnVel; };
	struct __attribute__((packed)) { u8 _pad_turnStrength[0x57C]; float turnStrength; };
	struct __attribute__((packed)) { u8 _pad_topSpeed[0x580]; float topSpeed; };
} ObjState_SnowBike;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_SpellStone {
	byte state;
} ObjState_SpellStone;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_Staff {
	struct __attribute__((packed)) { u8 _pad_unk54[0x54]; float unk54; };
	struct __attribute__((packed)) { u8 _pad_unk5C[0x5C]; float unk5C; };
	struct __attribute__((packed)) { u8 _pad_unk64[0x64]; float unk64; };
	struct __attribute__((packed)) { u8 _pad_unk6C[0x6C]; float unk6C; };
	struct __attribute__((packed)) { u8 _pad_unk74[0x74]; float unk74; };
	struct __attribute__((packed)) { u8 _pad_unk7C[0x7C]; float unk7C; };
	struct __attribute__((packed)) { u8 _pad_unkB0[0xB0]; short unkB0; };
	struct __attribute__((packed)) { u8 _pad_glowColor[0xBA]; StaffGlowColor glowColor; };
	struct __attribute__((packed)) { u8 _pad_glowStyle[0xBB]; undefined1 glowStyle; };
} ObjState_Staff;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_Timer {
	float time;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; pointer unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_state[0xC]; byte state; };
	struct __attribute__((packed)) { u8 _pad_flags[0xD]; byte flags; };
} ObjState_Timer;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_Tricky {
	byte* energy;
	struct __attribute__((packed)) { u8 _pad_obj_0x4[0x4]; int obj_0x4; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; byte unk08; };
	struct __attribute__((packed)) { u8 _pad_unk09[0x9]; byte unk09; };
	struct __attribute__((packed)) { u8 _pad_unk0A[0xA]; byte unk0A; };
	struct __attribute__((packed)) { u8 _pad_enabledCommands[0xB]; byte enabledCommands; };
	struct __attribute__((packed)) { u8 _pad_unk0D[0xD]; char unk0D; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; float unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; float unk14; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; float unk18; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; byte* unk20; };
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; int unk24; };
	struct __attribute__((packed)) { u8 _pad_unk28[0x28]; int unk28; };
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; byte* unk2C; };
	struct __attribute__((packed)) { u8 _pad_unk30[0x30]; byte* unk30; };
	struct __attribute__((packed)) { u8 _pad_unk34[0x34]; byte* unk34; };
	struct __attribute__((packed)) { u8 _pad_unk38[0x38]; byte* unk38; };
	struct __attribute__((packed)) { u8 _pad_unk3C[0x3C]; byte* unk3C; };
	struct __attribute__((packed)) { u8 _pad_unk40[0x40]; byte* unk40; };
	struct __attribute__((packed)) { u8 _pad_unk44[0x44]; byte* unk44; };
	struct __attribute__((packed)) { u8 _pad_unk48[0x48]; byte* unk48; };
	struct __attribute__((packed)) { u8 _pad_unk4C[0x4C]; byte* unk4C; };
	struct __attribute__((packed)) { u8 _pad_unk50[0x50]; byte* unk50; };
	struct __attribute__((packed)) { u8 _pad_flags_0x54[0x54]; TrickyStateFlags54 flags_0x54; };
	struct __attribute__((packed)) { u8 _pad_unk58[0x58]; byte unk58; };
	struct __attribute__((packed)) { u8 _pad_unk5A[0x5A]; short unk5A; };
	struct __attribute__((packed)) { u8 _pad_objId_0x5c[0x5C]; int objId_0x5c; };
	struct __attribute__((packed)) { u8 _pad_unk60[0x60]; float unk60; };
	struct __attribute__((packed)) { u8 _pad_unk8C[0x8C]; byte* unk8C; };
	struct __attribute__((packed)) { u8 _pad_unk90[0x90]; byte* unk90; };
	struct __attribute__((packed)) { u8 _pad_unk94[0x94]; byte* unk94; };
	struct __attribute__((packed)) { u8 _pad_unkD2[0xD2]; undefined2 unkD2; };
	struct __attribute__((packed)) { u8 _pad_unkE0[0xE0]; vec3f unkE0; };
	struct __attribute__((packed)) { u8 _pad_unk2AC[0x2AC]; float unk2AC; };
	struct __attribute__((packed)) { u8 _pad_unk2B0[0x2B0]; float unk2B0; };
	struct __attribute__((packed)) { u8 _pad_unk2B4[0x2B4]; byte* unk2B4; };
	struct __attribute__((packed)) { u8 _pad_hits[0x358]; char hits; };
	struct __attribute__((packed)) { u8 _pad_unk37C[0x37C]; byte* unk37C; };
	struct __attribute__((packed)) { u8 _pad_unk380[0x380]; byte* unk380; };
	struct __attribute__((packed)) { u8 _pad_unk384[0x384]; byte* unk384; };
	struct __attribute__((packed)) { u8 _pad_obj_0x700[0x700]; ObjInstance* obj_0x700; };
	struct __attribute__((packed)) { u8 _pad_unk71C[0x71C]; float unk71C; };
	struct __attribute__((packed)) { u8 _pad_unk720[0x720]; float unk720; };
	struct __attribute__((packed)) { u8 _pad_unk724[0x724]; undefined* unk724; };
	struct __attribute__((packed)) { u8 _pad_unk728[0x728]; undefined* unk728; };
	struct __attribute__((packed)) { u8 _pad_unk734[0x734]; byte unk734; };
	struct __attribute__((packed)) { u8 _pad_unk740[0x740]; byte* unk740; };
	struct __attribute__((packed)) { u8 _pad_unk748[0x748]; int unk748; };
	struct __attribute__((packed)) { u8 _pad_unk74C[0x74C]; char unk74C; };
	struct __attribute__((packed)) { u8 _pad_unk74D[0x74D]; char unk74D; };
	struct __attribute__((packed)) { u8 _pad_unk74E[0x74E]; char unk74E; };
	struct __attribute__((packed)) { u8 _pad_numCommPresent[0x798]; byte numCommPresent; };
	struct __attribute__((packed)) { u8 _pad_unk79C[0x79C]; float unk79C; };
	struct __attribute__((packed)) { u8 _pad_unk7A0[0x7A0]; float unk7A0; };
	struct __attribute__((packed)) { u8 _pad_unk7A8[0x7A8]; byte* unk7A8; };
	struct __attribute__((packed)) { u8 _pad_unk7B0[0x7B0]; byte* unk7B0; };
	struct __attribute__((packed)) { u8 _pad_unk7B8[0x7B8]; ObjInstance* unk7B8; };
	struct __attribute__((packed)) { u8 _pad_unk7BC[0x7BC]; byte unk7BC; };
	struct __attribute__((packed)) { u8 _pad_unk7C0[0x7C0]; float unk7C0; };
	struct __attribute__((packed)) { u8 _pad_unk7C4[0x7C4]; float unk7C4; };
	struct __attribute__((packed)) { u8 _pad_unk7C8[0x7C8]; float unk7C8; };
	struct __attribute__((packed)) { u8 _pad_badge[0x7CC]; ObjInstance* badge; };
	struct __attribute__((packed)) { u8 _pad_unk808[0x808]; float unk808; };
	struct __attribute__((packed)) { u8 _pad_unk80C[0x80C]; byte* unk80C; };
	struct __attribute__((packed)) { u8 _pad_unk810[0x810]; byte* unk810; };
	struct __attribute__((packed)) { u8 _pad_unk814[0x814]; byte* unk814; };
	struct __attribute__((packed)) { u8 _pad_unk81A[0x81A]; short unk81A; };
	struct __attribute__((packed)) { u8 _pad_unk827[0x827]; char unk827; };
	struct __attribute__((packed)) { u8 _pad_unk828[0x828]; float unk828; };
	struct __attribute__((packed)) { u8 _pad_energy82D[0x82D]; byte energy82D; };
	struct __attribute__((packed)) { u8 _pad_unk82E[0x82E]; byte unk82E; };
} ObjState_Tricky;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_VFP_flamepoint {
	undefined2 activated;
	struct __attribute__((packed)) { u8 _pad_gameBit02[0x2]; undefined2 gameBit02; };
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; char unk04; };
	struct __attribute__((packed)) { u8 _pad_flags05[0x5]; byte flags05; };
} ObjState_VFP_flamepoint;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_WarpStone {
	ObjInstance* obj00;
	struct __attribute__((packed)) { u8 _pad_particleFxTimer[0x4]; float particleFxTimer; };
	struct __attribute__((packed)) { u8 _pad_hitboxIdx[0x8]; byte hitboxIdx; };
	struct __attribute__((packed)) { u8 _pad_unk09[0x9]; byte unk09; };
	struct __attribute__((packed)) { u8 _pad_unk0A[0xA]; byte unk0A; };
	struct __attribute__((packed)) { u8 _pad_bit0E[0xE]; GameBit16 bit0E; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; char unk14; };
	struct __attribute__((packed)) { u8 _pad_unk44[0x44]; EyeAnimStruct unk44; };
	struct __attribute__((packed)) { u8 _pad_flagsD4[0xD4]; byte flagsD4; };
	struct __attribute__((packed)) { u8 _pad_flagsD5[0xD5]; byte flagsD5; };
} ObjState_WarpStone;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_WM_LevelControl {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; undefined2 unk04; };
	struct __attribute__((packed)) { u8 _pad_unk06[0x6]; ushort unk06; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; undefined2 unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0A[0xA]; byte unk0A; };
	struct __attribute__((packed)) { u8 _pad_unk0B[0xB]; byte unk0B; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; int unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; byte unk14; };
} ObjState_WM_LevelControl;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_WM_SeqPoint {
	float trigDist;
	struct __attribute__((packed)) { u8 _pad_bitSeqStarted[0x4]; GameBit16 bitSeqStarted; };
	struct __attribute__((packed)) { u8 _pad_bitStart[0x6]; GameBit16 bitStart; };
	struct __attribute__((packed)) { u8 _pad_seqIdx[0x8]; short seqIdx; };
	struct __attribute__((packed)) { u8 _pad_unk0A[0xA]; undefined2 unk0A; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; byte unk0C; };
	struct __attribute__((packed)) { u8 _pad_seqStarted[0xD]; byte seqStarted; };
	struct __attribute__((packed)) { u8 _pad_action[0xE]; byte action; };
	struct __attribute__((packed)) { u8 _pad_unk0F[0xF]; byte unk0F; };
} ObjState_WM_SeqPoint;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_WM_SpiritPlace {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; undefined4 unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; undefined2 unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0A[0xA]; undefined2 unk0A; };
	struct __attribute__((packed)) { u8 _pad_bitEnabled[0xC]; GameBit16 bitEnabled; };
	struct __attribute__((packed)) { u8 _pad_bitVisible[0xE]; GameBit16 bitVisible; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; short unk10; };
	struct __attribute__((packed)) { u8 _pad_flags12[0x12]; byte flags12; };
	struct __attribute__((packed)) { u8 _pad_mapAct[0x13]; byte mapAct; };
	struct __attribute__((packed)) { u8 _pad_showTimer[0x14]; byte showTimer; };
	struct __attribute__((packed)) { u8 _pad_flags15[0x15]; byte flags15; };
} ObjState_WM_SpiritPlace;
//include/sfa/xml/Game/GameObject/ObjState.xml
typedef union ObjState_WORLDplanet {
	struct __attribute__((packed)) { u8 _pad_nextCometTimer[0x6]; short nextCometTimer; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; byte unk08; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; byte unk10; };
	struct __attribute__((packed)) { u8 _pad_unk11[0x11]; byte unk11; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; uint unk14; };
} ObjState_WORLDplanet;
