//include/sfa/xml/Game/GameObject/Player/AnimState.xml
typedef enum { //type:u8
	PlayerAnimStateFlags3F0_FallingOrLanding = 0x4,
	PlayerAnimStateFlags3F0_Rolling = 0x10,
	PlayerAnimStateFlags3F0_Stopping = 0x80,
	PlayerAnimStateFlags3F0_MaybeInAir = 0x8,
	PlayerAnimStateFlags3F0_Swimming = 0x20,
	PlayerAnimStateFlags3F0_Shielding = 0x2,
	PlayerAnimStateFlags3F0_TurningAroundFast = 0x40,
	PlayerAnimStateFlags3F0_LedgeRelated = 0x1,
} PlayerAnimStateFlags3F0;
//include/sfa/xml/Game/GameObject/Player/AnimState.xml
typedef enum { //type:u8
	PlayerAnimStateFlags3F1_NotMoved = 0x8,
	PlayerAnimStateFlags3F1_HoldLRelated = 0x20,
	PlayerAnimStateFlags3F1_OnGround = 0x1,
	PlayerAnimStateFlags3F1_HoldingL = 0x10,
} PlayerAnimStateFlags3F1;
//include/sfa/xml/Game/GameObject/Player/AnimState.xml
typedef enum { //type:u8
	PlayerAnimStateFlags3F2_QuickTurnFlag_04 = 0x4,
	PlayerAnimStateFlags3F2_QuickTurnFlag_01 = 0x1,
	PlayerAnimStateFlags3F2_InCutscene = 0x20,
	PlayerAnimStateFlags3F2_FallDamage = 0x2,
	PlayerAnimStateFlags3F2_DoEyeAnims = 0x40,
	PlayerAnimStateFlags3F2_MaybeTakingDamageOrTalking = 0x8,
	PlayerAnimStateFlags3F2_StartingToMove = 0x10,
} PlayerAnimStateFlags3F2;
//include/sfa/xml/Game/GameObject/Player/AnimState.xml
typedef enum { //type:u8
	PlayerAnimStateFlags3F3_IsDisguised = 0x8,
	PlayerAnimStateFlags3F3_FallingScreaming = 0x20,
	PlayerAnimStateFlags3F3_IsDead = 0x2,
} PlayerAnimStateFlags3F3;
//include/sfa/xml/Game/GameObject/Player/AnimState.xml
typedef enum { //type:u8
	PlayerAnimStateFlags3F4_IsDisguised = 0x80,
	PlayerAnimStateFlags3F4_StaffInHand = 0x20,
	PlayerAnimStateFlags3F4_Flailing = 0x10,
	PlayerAnimStateFlags3F4_HaveStaff = 0x40,
	PlayerAnimStateFlags3F4_StaffOnBack = 0x8,
} PlayerAnimStateFlags3F4;
//include/sfa/xml/Game/GameObject/Player/AnimState.xml
typedef enum { //type:u8
	PlayerAnimStateFlags3F6_ShieldingInCombat = 0x20,
} PlayerAnimStateFlags3F6;
//include/sfa/xml/Game/GameObject/Player/AnimState.xml
typedef enum { //type:u32
	PlayerStateFlags310_HoldingObj = 0x4000,
	PlayerStateFlags310_Falling = 0x1,
} PlayerStateFlags310;
//include/sfa/xml/Game/GameObject/Player/AnimState.xml
typedef enum { //type:u32
	PlayerStateFlags360_Locked = 0x200000,
} PlayerStateFlags360;
//include/sfa/xml/Game/GameObject/Player/AnimState.xml
typedef union ObjState_Player {
	byte timer;
	struct __attribute__((packed)) { u8 _pad_buyItem[0x1]; PlayerBuyItemEnum buyItem; };
	struct __attribute__((packed)) { u8 _pad_scale_0x4[0x4]; uint scale_0x4; };
	struct __attribute__((packed)) { u8 _pad_duration[0x8]; undefined* duration; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; float unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; float unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; short unk14; };
	struct __attribute__((packed)) { u8 _pad_unk16[0x16]; short unk16; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; short unk18; };
	struct __attribute__((packed)) { u8 _pad_rotXdelta[0x1A]; short rotXdelta; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; float unk1C; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; float unk20; };
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; float unk24; };
	struct __attribute__((packed)) { u8 _pad_unk28[0x28]; float unk28; };
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; float unk2C; };
	struct __attribute__((packed)) { u8 _pad_unk30[0x30]; short unk30; };
	struct __attribute__((packed)) { u8 _pad_unk34[0x34]; undefined2 unk34; };
	struct __attribute__((packed)) { u8 _pad_state[0x36]; AnimState state; };
	struct __attribute__((packed)) { u8 _pad_flags_0x37[0x37]; byte flags_0x37; };
	struct __attribute__((packed)) { u8 _pad_unk38[0x38]; SoundId unk38; };
	struct __attribute__((packed)) { u8 _pad_unk3C[0x3C]; float unk3C; };
	struct __attribute__((packed)) { u8 _pad_unk40[0x40]; vec3f unk40; };
	struct __attribute__((packed)) { u8 _pad_scale_0x4c[0x4C]; float scale_0x4c; };
	struct __attribute__((packed)) { u8 _pad_unk50[0x50]; float unk50; };
	struct __attribute__((packed)) { u8 _pad_unk54[0x54]; float unk54; };
	struct __attribute__((packed)) { u8 _pad_unk58[0x58]; short unk58; };
	struct __attribute__((packed)) { u8 _pad_unk5A[0x5A]; s16 unk5A; };
	struct __attribute__((packed)) { u8 _pad_unk5C[0x5C]; float unk5C; };
	struct __attribute__((packed)) { u8 _pad_unk60[0x60]; short unk60; };
	struct __attribute__((packed)) { u8 _pad_nActions[0x62]; short nActions; };
	struct __attribute__((packed)) { u8 _pad_seqSize[0x64]; short seqSize; };
	struct __attribute__((packed)) { u8 _pad_curAction[0x66]; short curAction; };
	struct __attribute__((packed)) { u8 _pad_stopAtAction[0x68]; short stopAtAction; };
	struct __attribute__((packed)) { u8 _pad_unk6A[0x6A]; short unk6A; };
	struct __attribute__((packed)) { u8 _pad_unk6C[0x6C]; undefined2 unk6C; };
	struct __attribute__((packed)) { u8 _pad_flags6E[0x6E]; ObjState_AnimatedObj_Flags6E flags6E; };
	struct __attribute__((packed)) { u8 _pad_unk70[0x70]; float unk70; };
	struct __attribute__((packed)) { u8 _pad_unk74[0x74]; float unk74; };
	struct __attribute__((packed)) { u8 _pad_unk78[0x78]; byte unk78; };
	struct __attribute__((packed)) { u8 _pad_unk79[0x79]; byte unk79; };
	struct __attribute__((packed)) { u8 _pad_unk7A[0x7A]; byte unk7A; };
	struct __attribute__((packed)) { u8 _pad_unk7B[0x7B]; byte unk7B; };
	struct __attribute__((packed)) { u8 _pad_unk7C[0x7C]; char unk7C; };
	struct __attribute__((packed)) { u8 _pad_unk7E[0x7E]; byte unk7E; };
	struct __attribute__((packed)) { u8 _pad_flags_0x7f[0x7F]; byte flags_0x7f; };
	struct __attribute__((packed)) { u8 _pad_unk80[0x80]; float unk80; };
	struct __attribute__((packed)) { u8 _pad_unk84[0x84]; int unk84; };
	struct __attribute__((packed)) { u8 _pad_unk8B[0x8B]; byte unk8B; };
	struct __attribute__((packed)) { u8 _pad_seqTex5State[0x8D]; byte seqTex5State; };
	struct __attribute__((packed)) { u8 _pad_seqEyeState[0x8E]; byte seqEyeState; };
	struct __attribute__((packed)) { u8 _pad_acRomTab[0x94]; AnimCurvCommand* acRomTab; };
	struct __attribute__((packed)) { u8 _pad_pSeqEnd[0x98]; pointer pSeqEnd; };
	struct __attribute__((packed)) { u8 _pad_unk9C[0x9C]; short unk9C; };
	struct __attribute__((packed)) { u8 _pad_unk9E[0x9E]; short unk9E; };
	struct __attribute__((packed)) { u8 _pad_unkA0[0xA0]; short unkA0; };
	struct __attribute__((packed)) { u8 _pad_unkA2[0xA2]; short unkA2; };
	struct __attribute__((packed)) { u8 _pad_unkA4[0xA4]; short unkA4; };
	struct __attribute__((packed)) { u8 _pad_unkA6[0xA6]; short unkA6; };
	struct __attribute__((packed)) { u8 _pad_unkA8[0xA8]; short unkA8; };
	struct __attribute__((packed)) { u8 _pad_unkAA[0xAA]; short unkAA; };
	struct __attribute__((packed)) { u8 _pad_unkAC[0xAC]; float unkAC; };
	struct __attribute__((packed)) { u8 _pad_unkB0[0xB0]; float unkB0; };
	struct __attribute__((packed)) { u8 _pad_unkB4[0xB4]; short unkB4; };
	struct __attribute__((packed)) { u8 _pad_unkB6[0xB6]; short unkB6; };
	struct __attribute__((packed)) { u8 _pad_unkB8[0xB8]; short unkB8; };
	struct __attribute__((packed)) { u8 _pad_unkBA[0xBA]; short unkBA; };
	struct __attribute__((packed)) { u8 _pad_unkBC[0xBC]; short unkBC; };
	struct __attribute__((packed)) { u8 _pad_unkBE[0xBE]; short unkBE; };
	struct __attribute__((packed)) { u8 _pad_unkC0[0xC0]; short unkC0; };
	struct __attribute__((packed)) { u8 _pad_unkC2[0xC2]; short unkC2; };
	struct __attribute__((packed)) { u8 _pad_flagsC4[0xC4]; byte flagsC4; };
	struct __attribute__((packed)) { u8 _pad_unkC8[0xC8]; short unkC8; };
	struct __attribute__((packed)) { u8 _pad_unkCA[0xCA]; short unkCA; };
	struct __attribute__((packed)) { u8 _pad_unkCC[0xCC]; short unkCC; };
	struct __attribute__((packed)) { u8 _pad_unkCE[0xCE]; short unkCE; };
	struct __attribute__((packed)) { u8 _pad_unkD0[0xD0]; short unkD0; };
	struct __attribute__((packed)) { u8 _pad_unkD2[0xD2]; short unkD2; };
	struct __attribute__((packed)) { u8 _pad_unkD4[0xD4]; undefined2 unkD4; };
	struct __attribute__((packed)) { u8 _pad_unkD6[0xD6]; undefined2 unkD6; };
	struct __attribute__((packed)) { u8 _pad_unkD8[0xD8]; short unkD8; };
	struct __attribute__((packed)) { u8 _pad_unkDA[0xDA]; short unkDA; };
	struct __attribute__((packed)) { u8 _pad_unkDC[0xDC]; short unkDC; };
	struct __attribute__((packed)) { u8 _pad_unkDE[0xDE]; short unkDE; };
	struct __attribute__((packed)) { u8 _pad_unkE0[0xE0]; pointer unkE0; };
	struct __attribute__((packed)) { u8 _pad_unkE4[0xE4]; pointer unkE4; };
	struct __attribute__((packed)) { u8 _pad_unkE8[0xE8]; float unkE8; };
	struct __attribute__((packed)) { u8 _pad_unkEC[0xEC]; float unkEC; };
	struct __attribute__((packed)) { u8 _pad_unkF0[0xF0]; float unkF0; };
	struct __attribute__((packed)) { u8 _pad_unkF4[0xF4]; pointer unkF4; };
	struct __attribute__((packed)) { u8 _pad_unkF8[0xF8]; pointer unkF8; };
	struct __attribute__((packed)) { u8 _pad_unkFC[0xFC]; pointer unkFC; };
	struct __attribute__((packed)) { u8 _pad_unk100[0x100]; pointer unk100; };
	struct __attribute__((packed)) { u8 _pad_unk104[0x104]; pointer unk104; };
	struct __attribute__((packed)) { u8 _pad_unk110[0x110]; int unk110; };
	struct __attribute__((packed)) { u8 _pad_unk114[0x114]; short unk114; };
	struct __attribute__((packed)) { u8 _pad_unk116[0x116]; short unk116; };
	struct __attribute__((packed)) { u8 _pad_unk118[0x118]; vec3f unk118; };
	struct __attribute__((packed)) { u8 _pad_unk128[0x128]; float unk128; };
	struct __attribute__((packed)) { u8 _pad_unk136[0x136]; byte unk136; };
	struct __attribute__((packed)) { u8 _pad_unk14C[0x14C]; float unk14C; };
	struct __attribute__((packed)) { u8 _pad_unk150[0x150]; float unk150; };
	struct __attribute__((packed)) { u8 _pad_unk154[0x154]; float unk154; };
	struct __attribute__((packed)) { u8 _pad_unk158[0x158]; float unk158; };
	struct __attribute__((packed)) { u8 _pad_unk15C[0x15C]; float unk15C; };
	struct __attribute__((packed)) { u8 _pad_unk160[0x160]; float unk160; };
	struct __attribute__((packed)) { u8 _pad_unk164[0x164]; float unk164; };
	struct __attribute__((packed)) { u8 _pad_unk168[0x168]; float unk168; };
	struct __attribute__((packed)) { u8 _pad_unk16C[0x16C]; float unk16C; };
	struct __attribute__((packed)) { u8 _pad_unk170[0x170]; float unk170; };
	struct __attribute__((packed)) { u8 _pad_unk174[0x174]; float unk174; };
	struct __attribute__((packed)) { u8 _pad_unk178[0x178]; float unk178; };
	struct __attribute__((packed)) { u8 _pad_unk17C[0x17C]; float unk17C; };
	struct __attribute__((packed)) { u8 _pad_unk180[0x180]; float unk180; };
	struct __attribute__((packed)) { u8 _pad_unk184[0x184]; float unk184; };
	struct __attribute__((packed)) { u8 _pad_unk188[0x188]; float unk188; };
	struct __attribute__((packed)) { u8 _pad_unk18C[0x18C]; float unk18C; };
	struct __attribute__((packed)) { u8 _pad_unk190[0x190]; float unk190; };
	struct __attribute__((packed)) { u8 _pad_unk19C[0x19C]; short unk19C; };
	struct __attribute__((packed)) { u8 _pad_unk1A8[0x1A8]; float unk1A8; };
	struct __attribute__((packed)) { u8 _pad_unk1AC[0x1AC]; float unk1AC; };
	struct __attribute__((packed)) { u8 _pad_unk1B0[0x1B0]; float unk1B0; };
	struct __attribute__((packed)) { u8 _pad_unk1B4[0x1B4]; float unk1B4; };
	struct __attribute__((packed)) { u8 _pad_unk1B8[0x1B8]; float unk1B8; };
	struct __attribute__((packed)) { u8 _pad_unk1BC[0x1BC]; float unk1BC; };
	struct __attribute__((packed)) { u8 _pad_unk1C0[0x1C0]; float unk1C0; };
	struct __attribute__((packed)) { u8 _pad_unk1D4[0x1D4]; float unk1D4; };
	struct __attribute__((packed)) { u8 _pad_unk1D8[0x1D8]; float unk1D8; };
	struct __attribute__((packed)) { u8 _pad_unk1F4[0x1F4]; float unk1F4; };
	struct __attribute__((packed)) { u8 _pad_unk1F8[0x1F8]; float unk1F8; };
	struct __attribute__((packed)) { u8 _pad_unk204[0x204]; float unk204; };
	struct __attribute__((packed)) { u8 _pad_unk208[0x208]; float unk208; };
	struct __attribute__((packed)) { u8 _pad_unk224[0x224]; float unk224; };
	struct __attribute__((packed)) { u8 _pad_unk230[0x230]; dword unk230; };
	struct __attribute__((packed)) { u8 _pad_unk238[0x238]; float unk238; };
	struct __attribute__((packed)) { u8 _pad_unk240[0x240]; SoundId unk240; };
	struct __attribute__((packed)) { u8 _pad_unk25C[0x25C]; byte unk25C; };
	struct __attribute__((packed)) { u8 _pad_unk25F[0x25F]; char unk25F; };
	struct __attribute__((packed)) { u8 _pad_unk26C[0x26C]; undefined2 unk26C; };
	struct __attribute__((packed)) { u8 _pad_unk26E[0x26E]; undefined2 unk26E; };
	struct __attribute__((packed)) { u8 _pad_unk270[0x270]; short unk270; };
	struct __attribute__((packed)) { u8 _pad_curState[0x274]; PlayerStateEnum curState; };
	struct __attribute__((packed)) { u8 _pad_prevState[0x276]; PlayerStateEnum prevState; };
	struct __attribute__((packed)) { u8 _pad_unk278[0x278]; undefined2 unk278; };
	struct __attribute__((packed)) { u8 _pad_stateChanged[0x27A]; byte stateChanged; };
	struct __attribute__((packed)) { u8 _pad_unk27B[0x27B]; byte unk27B; };
	struct __attribute__((packed)) { u8 _pad_unk27C[0x27C]; float* unk27C; };
	struct __attribute__((packed)) { u8 _pad_distance[0x280]; float distance; };
	struct __attribute__((packed)) { u8 _pad_sidewaysFallPush[0x284]; float sidewaysFallPush; };
	struct __attribute__((packed)) { u8 _pad_unk288[0x288]; float unk288; };
	struct __attribute__((packed)) { u8 _pad_stickYfloat_0x28c[0x28C]; float stickYfloat_0x28c; };
	struct __attribute__((packed)) { u8 _pad_stickXfloat_0x290[0x290]; float stickXfloat_0x290; };
	struct __attribute__((packed)) { u8 _pad_velXZ[0x294]; float velXZ; };
	struct __attribute__((packed)) { u8 _pad_unk298[0x298]; float unk298; };
	struct __attribute__((packed)) { u8 _pad_unk29C[0x29C]; float unk29C; };
	struct __attribute__((packed)) { u8 _pad_animSpeed[0x2A0]; float animSpeed; };
	struct __attribute__((packed)) { u8 _pad_unk2A4[0x2A4]; float unk2A4; };
	struct __attribute__((packed)) { u8 _pad_unk2A8[0x2A8]; float unk2A8; };
	struct __attribute__((packed)) { u8 _pad_unk2AC[0x2AC]; float unk2AC; };
	struct __attribute__((packed)) { u8 _pad_unk2B4[0x2B4]; float unk2B4; };
	struct __attribute__((packed)) { u8 _pad_unk2B8[0x2B8]; float unk2B8; };
	struct __attribute__((packed)) { u8 _pad_curveDist02BC[0x2BC]; float curveDist02BC; };
	struct __attribute__((packed)) { u8 _pad_unk2D0[0x2D0]; ObjInstance* unk2D0; };
	struct __attribute__((packed)) { u8 _pad_unk2E8[0x2E8]; dword unk2E8; };
	struct __attribute__((packed)) { u8 _pad_unk2F4[0x2F4]; vec3f unk2F4; };
	struct __attribute__((packed)) { u8 _pad_unk300[0x300]; float unk300; };
	struct __attribute__((packed)) { u8 _pad_onStateChange[0x304]; undefined* onStateChange; };
	struct __attribute__((packed)) { u8 _pad_onNextStateChange[0x308]; undefined* onNextStateChange; };
	struct __attribute__((packed)) { u8 _pad_flags310[0x310]; PlayerStateFlags310 flags310; };
	struct __attribute__((packed)) { u8 _pad_flags0314[0x314]; uint flags0314; };
	struct __attribute__((packed)) { u8 _pad_unk318[0x318]; uint unk318; };
	struct __attribute__((packed)) { u8 _pad_unk31C[0x31C]; uint unk31C; };
	struct __attribute__((packed)) { u8 _pad_unk324[0x324]; float unk324; };
	struct __attribute__((packed)) { u8 _pad_unk330[0x330]; short unk330; };
	struct __attribute__((packed)) { u8 _pad_unk334[0x334]; float unk334; };
	struct __attribute__((packed)) { u8 _pad_unk338[0x338]; undefined2 unk338; };
	struct __attribute__((packed)) { u8 _pad_unk33B[0x33B]; byte unk33B; };
	struct __attribute__((packed)) { u8 _pad_curveObjId[0x33C]; int curveObjId; };
	struct __attribute__((packed)) { u8 _pad_unk340[0x340]; undefined4 unk340; };
	struct __attribute__((packed)) { u8 _pad_curveAction[0x344]; byte curveAction; };
	struct __attribute__((packed)) { u8 _pad_unk349[0x349]; byte unk349; };
	struct __attribute__((packed)) { u8 _pad_flags034C[0x34C]; byte flags034C; };
	struct __attribute__((packed)) { u8 _pad_unk34D[0x34D]; byte unk34D; };
	struct __attribute__((packed)) { u8 _pad_unk356[0x356]; byte unk356; };
	struct __attribute__((packed)) { u8 _pad_unk358[0x358]; byte unk358; };
	struct __attribute__((packed)) { u8 _pad_charState[0x35C]; PlayerCharState* charState; };
	struct __attribute__((packed)) { u8 _pad_flags360[0x360]; PlayerStateFlags360 flags360; };
	struct __attribute__((packed)) { u8 _pad_eyeAnim[0x364]; EyeAnimStruct eyeAnim; };
	struct __attribute__((packed)) { u8 _pad_unk3C4[0x3C4]; float unk3C4; };
	struct __attribute__((packed)) { u8 _pad_unk3C8[0x3C8]; float unk3C8; };
	struct __attribute__((packed)) { u8 _pad_unk3CC[0x3CC]; float unk3CC; };
	struct __attribute__((packed)) { u8 _pad_unk3D0[0x3D0]; float unk3D0; };
	struct __attribute__((packed)) { u8 _pad_unk3D4[0x3D4]; float unk3D4; };
	struct __attribute__((packed)) { u8 _pad_unk3D8[0x3D8]; float unk3D8; };
	struct __attribute__((packed)) { u8 _pad_weaponVar3DC[0x3DC]; pointer weaponVar3DC; };
	struct __attribute__((packed)) { u8 _pad_moneyAdded[0x3E8]; byte moneyAdded; };
	struct __attribute__((packed)) { u8 _pad_flags3F0[0x3F0]; PlayerAnimStateFlags3F0 flags3F0; };
	struct __attribute__((packed)) { u8 _pad_flags3F1[0x3F1]; PlayerAnimStateFlags3F1 flags3F1; };
	struct __attribute__((packed)) { u8 _pad_flags3F2[0x3F2]; PlayerAnimStateFlags3F2 flags3F2; };
	struct __attribute__((packed)) { u8 _pad_flags3F3[0x3F3]; PlayerAnimStateFlags3F3 flags3F3; };
	struct __attribute__((packed)) { u8 _pad_flags3F4[0x3F4]; PlayerAnimStateFlags3F4 flags3F4; };
	struct __attribute__((packed)) { u8 _pad_unk3F5[0x3F5]; byte unk3F5; };
	struct __attribute__((packed)) { u8 _pad_flags3F6[0x3F6]; PlayerAnimStateFlags3F6 flags3F6; };
	struct __attribute__((packed)) { u8 _pad_fallDistance[0x3F7]; byte fallDistance; };
	struct __attribute__((packed)) { u8 _pad_animIds[0x3F8]; pointer animIds; };
	struct __attribute__((packed)) { u8 _pad_prevAnimIds[0x3FC]; pointer prevAnimIds; };
	struct __attribute__((packed)) { u8 _pad_pAnimSpeeds[0x400]; float* pAnimSpeeds; };
	struct __attribute__((packed)) { u8 _pad_fallForwardPush[0x404]; float fallForwardPush; };
	struct __attribute__((packed)) { u8 _pad_unk408[0x408]; float unk408; };
	struct __attribute__((packed)) { u8 _pad_unk40C[0x40C]; byte unk40C; };
	struct __attribute__((packed)) { u8 _pad_unk40D[0x40D]; byte unk40D; };
	struct __attribute__((packed)) { u8 _pad_time_0x410[0x410]; float time_0x410; };
	struct __attribute__((packed)) { u8 _pad_unk414[0x414]; float unk414; };
	struct __attribute__((packed)) { u8 _pad_unk420[0x420]; float unk420; };
	struct __attribute__((packed)) { u8 _pad_unk428[0x428]; float unk428; };
	struct __attribute__((packed)) { u8 _pad_unk42C[0x42C]; float unk42C; };
	struct __attribute__((packed)) { u8 _pad_unk430[0x430]; float unk430; };
	struct __attribute__((packed)) { u8 _pad_unk434[0x434]; float unk434; };
	struct __attribute__((packed)) { u8 _pad_unk438[0x438]; float unk438; };
	struct __attribute__((packed)) { u8 _pad_unk43C[0x43C]; float unk43C; };
	struct __attribute__((packed)) { u8 _pad_unk440[0x440]; float unk440; };
	struct __attribute__((packed)) { u8 _pad_unk450[0x450]; float* unk450; };
	struct __attribute__((packed)) { u8 _pad_unk454[0x454]; float* unk454; };
	struct __attribute__((packed)) { u8 _pad_unk458[0x458]; float* unk458; };
	struct __attribute__((packed)) { u8 _pad_unk45C[0x45C]; float* unk45C; };
	struct __attribute__((packed)) { u8 _pad_unk460[0x460]; float* unk460; };
	struct __attribute__((packed)) { u8 _pad_unk464[0x464]; int unk464; };
	struct __attribute__((packed)) { u8 _pad_curDeathObj[0x46C]; ObjInstance* curDeathObj; };
	struct __attribute__((packed)) { u8 _pad_unk470[0x470]; float unk470; };
	struct __attribute__((packed)) { u8 _pad_unk474[0x474]; uint unk474; };
	struct __attribute__((packed)) { u8 _pad_rotx1[0x478]; short rotx1; };
	struct __attribute__((packed)) { u8 _pad_unk47C[0x47C]; int unk47C; };
	struct __attribute__((packed)) { u8 _pad_unk480[0x480]; int unk480; };
	struct __attribute__((packed)) { u8 _pad_rotx2[0x484]; short rotx2; };
	struct __attribute__((packed)) { u8 _pad_unk488[0x488]; int unk488; };
	struct __attribute__((packed)) { u8 _pad_unk48C[0x48C]; int unk48C; };
	struct __attribute__((packed)) { u8 _pad_rot490[0x490]; short rot490; };
	struct __attribute__((packed)) { u8 _pad_rot492[0x492]; short rot492; };
	struct __attribute__((packed)) { u8 _pad_rotx3[0x494]; int rotx3; };
	struct __attribute__((packed)) { u8 _pad_unk498[0x498]; int unk498; };
	struct __attribute__((packed)) { u8 _pad_unk49C[0x49C]; int unk49C; };
	struct __attribute__((packed)) { u8 _pad_unk4A4[0x4A4]; int unk4A4; };
	struct __attribute__((packed)) { u8 _pad_unk4A8[0x4A8]; int unk4A8; };
	struct __attribute__((packed)) { u8 _pad_unk4AC[0x4AC]; uint unk4AC; };
	struct __attribute__((packed)) { u8 _pad_unk4B0[0x4B0]; float unk4B0; };
	struct __attribute__((packed)) { u8 _pad_unk4B4[0x4B4]; ushort unk4B4; };
	struct __attribute__((packed)) { u8 _pad_unk4B9[0x4B9]; int unk4B9; };
	struct __attribute__((packed)) { u8 _pad_pObj_0x4c4[0x4C4]; ObjInstance* pObj_0x4c4; };
	struct __attribute__((packed)) { u8 _pad_unk4C8[0x4C8]; float unk4C8; };
	struct __attribute__((packed)) { u8 _pad_unk4CC[0x4CC]; float unk4CC; };
	struct __attribute__((packed)) { u8 _pad_unk4D0[0x4D0]; short unk4D0; };
	struct __attribute__((packed)) { u8 _pad_unk4D2[0x4D2]; short unk4D2; };
	struct __attribute__((packed)) { u8 _pad_unk4D4[0x4D4]; short unk4D4; };
	struct __attribute__((packed)) { u8 _pad_unk4D6[0x4D6]; short unk4D6; };
	struct __attribute__((packed)) { u8 _pad_unk4D8[0x4D8]; short unk4D8; };
	struct __attribute__((packed)) { u8 _pad_unk4DA[0x4DA]; short unk4DA; };
	struct __attribute__((packed)) { u8 _pad_unk4DC[0x4DC]; short unk4DC; };
	struct __attribute__((packed)) { u8 _pad_angle4DE[0x4DE]; short angle4DE; };
	struct __attribute__((packed)) { u8 _pad_unk4E0[0x4E0]; short unk4E0; };
	struct __attribute__((packed)) { u8 _pad_unk4E4[0x4E4]; byte unk4E4; };
	struct __attribute__((packed)) { u8 _pad_unk54C[0x54C]; float unk54C; };
	struct __attribute__((packed)) { u8 _pad_unk550[0x550]; float unk550; };
	struct __attribute__((packed)) { u8 _pad_unk5A8[0x5A8]; float unk5A8; };
	struct __attribute__((packed)) { u8 _pad_unk5AC[0x5AC]; float unk5AC; };
	struct __attribute__((packed)) { u8 _pad_unk5B0[0x5B0]; float unk5B0; };
	struct __attribute__((packed)) { u8 _pad_unk5B4[0x5B4]; vec3f unk5B4; };
	struct __attribute__((packed)) { u8 _pad_velScale5C4[0x5C4]; float velScale5C4; };
	struct __attribute__((packed)) { u8 _pad_unk5CC[0x5CC]; float unk5CC; };
	struct __attribute__((packed)) { u8 _pad_pos5D4[0x5D4]; vec3f pos5D4; };
	struct __attribute__((packed)) { u8 _pad_unk604[0x604]; short unk604; };
	struct __attribute__((packed)) { u8 _pad_unk606[0x606]; byte unk606; };
	struct __attribute__((packed)) { u8 _pad_unk608[0x608]; byte unk608; };
	struct __attribute__((packed)) { u8 _pad_unk609[0x609]; byte unk609; };
	struct __attribute__((packed)) { u8 _pad_targetObj_0x684[0x684]; ObjInstance* targetObj_0x684; };
	struct __attribute__((packed)) { u8 _pad_unk688[0x688]; GameBit16 unk688; };
	struct __attribute__((packed)) { u8 _pad_unk6A4[0x6A4]; float unk6A4; };
	struct __attribute__((packed)) { u8 _pad_unk6A8[0x6A8]; float unk6A8; };
	struct __attribute__((packed)) { u8 _pad_unk6AC[0x6AC]; float unk6AC; };
	struct __attribute__((packed)) { u8 _pad_unk6B0[0x6B0]; float unk6B0; };
	struct __attribute__((packed)) { u8 _pad_stickX[0x6D0]; uint stickX; };
	struct __attribute__((packed)) { u8 _pad_stickY[0x6D4]; uint stickY; };
	struct __attribute__((packed)) { u8 _pad_stickYfloat[0x6D8]; float stickYfloat; };
	struct __attribute__((packed)) { u8 _pad_stickXfloat[0x6DC]; float stickXfloat; };
	struct __attribute__((packed)) { u8 _pad_btnsHeld[0x6E0]; GamepadButton btnsHeld; };
	struct __attribute__((packed)) { u8 _pad_btnsPressed[0x6E2]; GamepadButton btnsPressed; };
	struct __attribute__((packed)) { u8 _pad_btnsPressedNotBusy[0x6E4]; GamepadButton btnsPressedNotBusy; };
	struct __attribute__((packed)) { u8 _pad_pAnimIdsRide[0x6E8]; word* pAnimIdsRide; };
	struct __attribute__((packed)) { u8 _pad_unk6EC[0x6EC]; byte unk6EC; };
	struct __attribute__((packed)) { u8 _pad_unk6F0[0x6F0]; float unk6F0; };
	struct __attribute__((packed)) { u8 _pad_unk6F8[0x6F8]; float unk6F8; };
	struct __attribute__((packed)) { u8 _pad_unk700[0x700]; float unk700; };
	struct __attribute__((packed)) { u8 _pad_unk708[0x708]; float unk708; };
	struct __attribute__((packed)) { u8 _pad_unk710[0x710]; float unk710; };
	struct __attribute__((packed)) { u8 _pad_unk768[0x768]; float unk768; };
	struct __attribute__((packed)) { u8 _pad_unk76C[0x76C]; float unk76C; };
	struct __attribute__((packed)) { u8 _pad_unk770[0x770]; float unk770; };
	struct __attribute__((packed)) { u8 _pad_camHeightVar778[0x778]; float camHeightVar778; };
	struct __attribute__((packed)) { u8 _pad_frictionVar77C[0x77C]; float frictionVar77C; };
	struct __attribute__((packed)) { u8 _pad_unk788[0x788]; float unk788; };
	struct __attribute__((packed)) { u8 _pad_unk78C[0x78C]; float unk78C; };
	struct __attribute__((packed)) { u8 _pad_onFireTimer[0x79C]; float onFireTimer; };
	struct __attribute__((packed)) { u8 _pad_fireVar_0x7a0[0x7A0]; float fireVar_0x7a0; };
	struct __attribute__((packed)) { u8 _pad_fireVel[0x7A4]; float fireVel; };
	struct __attribute__((packed)) { u8 _pad_child_0x7a8[0x7A8]; byte child_0x7a8; };
	struct __attribute__((packed)) { u8 _pad_unk7AC[0x7AC]; byte unk7AC; };
	struct __attribute__((packed)) { u8 _pad_child_0x7b0[0x7B0]; ObjInstance* child_0x7b0; };
	struct __attribute__((packed)) { u8 _pad_aimX[0x7B8]; float aimX; };
	struct __attribute__((packed)) { u8 _pad_aimY[0x7BC]; float aimY; };
	struct __attribute__((packed)) { u8 _pad_shadowYOffset[0x7C8]; float shadowYOffset; };
	struct __attribute__((packed)) { u8 _pad_timeScale7CC[0x7CC]; float timeScale7CC; };
	struct __attribute__((packed)) { u8 _pad_timer7D0[0x7D0]; float timer7D0; };
	struct __attribute__((packed)) { u8 _pad_teleportAnimTimer[0x7D4]; float teleportAnimTimer; };
	struct __attribute__((packed)) { u8 _pad_unk7D8[0x7D8]; float unk7D8; };
	struct __attribute__((packed)) { u8 _pad_f7DC[0x7DC]; float f7DC; };
	struct __attribute__((packed)) { u8 _pad_unk7E0[0x7E0]; float unk7E0; };
	struct __attribute__((packed)) { u8 _pad_unk7EC[0x7EC]; undefined4 unk7EC; };
	struct __attribute__((packed)) { u8 _pad_ridingObject[0x7F0]; ObjInstance* ridingObject; };
	struct __attribute__((packed)) { u8 _pad_heldObject[0x7F8]; ObjInstance* heldObject; };
	struct __attribute__((packed)) { u8 _pad_unk7FC[0x7FC]; float unk7FC; };
	struct __attribute__((packed)) { u8 _pad_objHoldRelated800[0x800]; byte objHoldRelated800; };
	struct __attribute__((packed)) { u8 _pad_unk806[0x806]; undefined2 unk806; };
	struct __attribute__((packed)) { u8 _pad_unk808[0x808]; short unk808; };
	struct __attribute__((packed)) { u8 _pad_prevSpell[0x80A]; GameBit16 prevSpell; };
	struct __attribute__((packed)) { u8 _pad_pendingSpell[0x80C]; GameBit16 pendingSpell; };
	struct __attribute__((packed)) { u8 _pad_selectedSpell[0x80E]; GameBit16 selectedSpell; };
	struct __attribute__((packed)) { u8 _pad_frameCount_0x810[0x810]; ushort frameCount_0x810; };
	struct __attribute__((packed)) { u8 _pad_animTimer812[0x812]; short animTimer812; };
	struct __attribute__((packed)) { u8 _pad_animTimer814[0x814]; float animTimer814; };
	struct __attribute__((packed)) { u8 _pad_unk819[0x819]; byte unk819; };
	struct __attribute__((packed)) { u8 _pad_isFox[0x81A]; short isFox; };
	struct __attribute__((packed)) { u8 _pad_unk820[0x820]; float unk820; };
	struct __attribute__((packed)) { u8 _pad_unk82C[0x82C]; float unk82C; };
	struct __attribute__((packed)) { u8 _pad_unk830[0x830]; float unk830; };
	struct __attribute__((packed)) { u8 _pad_unk834[0x834]; float unk834; };
	struct __attribute__((packed)) { u8 _pad_unk838[0x838]; float unk838; };
	struct __attribute__((packed)) { u8 _pad_unk83C[0x83C]; float unk83C; };
	struct __attribute__((packed)) { u8 _pad_unk840[0x840]; float unk840; };
	struct __attribute__((packed)) { u8 _pad_unk844[0x844]; float unk844; };
	struct __attribute__((packed)) { u8 _pad_unk848[0x848]; float unk848; };
	struct __attribute__((packed)) { u8 _pad_prevY_84C[0x84C]; float prevY_84C; };
	struct __attribute__((packed)) { u8 _pad_offsetPrevY_850[0x850]; float offsetPrevY_850; };
	struct __attribute__((packed)) { u8 _pad_magicDurationTimer[0x854]; float magicDurationTimer; };
	struct __attribute__((packed)) { u8 _pad_unk85C[0x85C]; float unk85C; };
	struct __attribute__((packed)) { u8 _pad_unk86B[0x86B]; byte unk86B; };
	struct __attribute__((packed)) { u8 _pad_unk86C[0x86C]; char unk86C; };
	struct __attribute__((packed)) { u8 _pad_animIdx86F[0x86F]; byte animIdx86F; };
	struct __attribute__((packed)) { u8 _pad_ledgeGrabHeight[0x874]; float ledgeGrabHeight; };
	struct __attribute__((packed)) { u8 _pad_time_0x878[0x878]; float time_0x878; };
	struct __attribute__((packed)) { u8 _pad_time_0x87c[0x87C]; float time_0x87c; };
	struct __attribute__((packed)) { u8 _pad_time_0x880[0x880]; float time_0x880; };
	struct __attribute__((packed)) { u8 _pad_unk890[0x890]; float unk890; };
	struct __attribute__((packed)) { u8 _pad_unk894[0x894]; float unk894; };
	struct __attribute__((packed)) { u8 _pad_unk898[0x898]; undefined4 unk898; };
	struct __attribute__((packed)) { u8 _pad_unk89C[0x89C]; short unk89C; };
	struct __attribute__((packed)) { u8 _pad_unk8A0[0x8A0]; ushort unk8A0; };
	struct __attribute__((packed)) { u8 _pad_unk8A3[0x8A3]; byte unk8A3; };
	struct __attribute__((packed)) { u8 _pad_unk8A4[0x8A4]; byte unk8A4; };
	struct __attribute__((packed)) { u8 _pad_unk8A5[0x8A5]; byte unk8A5; };
	struct __attribute__((packed)) { u8 _pad_unk8A6[0x8A6]; byte unk8A6; };
	struct __attribute__((packed)) { u8 _pad_unk8A7[0x8A7]; byte unk8A7; };
	struct __attribute__((packed)) { u8 _pad_numWeapons[0x8A8]; byte numWeapons; };
	struct __attribute__((packed)) { u8 _pad_unk8B0[0x8B0]; byte unk8B0; };
	struct __attribute__((packed)) { u8 _pad_unk8B1[0x8B1]; byte unk8B1; };
	struct __attribute__((packed)) { u8 _pad_holdingStaff[0x8B3]; char holdingStaff; };
	struct __attribute__((packed)) { u8 _pad_staffState[0x8B4]; byte staffState; };
	struct __attribute__((packed)) { u8 _pad_unk8B8[0x8B8]; byte unk8B8; };
	struct __attribute__((packed)) { u8 _pad_unk8BE[0x8BE]; byte unk8BE; };
	struct __attribute__((packed)) { u8 _pad_unk8BF[0x8BF]; byte unk8BF; };
	struct __attribute__((packed)) { u8 _pad_action8C2[0x8C2]; byte action8C2; };
	struct __attribute__((packed)) { u8 _pad_unk8C3[0x8C3]; byte unk8C3; };
	struct __attribute__((packed)) { u8 _pad_unk8C4[0x8C4]; byte unk8C4; };
	struct __attribute__((packed)) { u8 _pad_unk8C5[0x8C5]; byte unk8C5; };
	struct __attribute__((packed)) { u8 _pad_unk8C6[0x8C6]; byte unk8C6; };
	struct __attribute__((packed)) { u8 _pad_haveSpells[0x8C7]; byte haveSpells; };
	struct __attribute__((packed)) { u8 _pad_unk8C8[0x8C8]; u8 unk8C8; };
	struct __attribute__((packed)) { u8 _pad_unk8C9[0x8C9]; byte unk8C9; };
	struct __attribute__((packed)) { u8 _pad_bDoTeleportAnim[0x8CA]; char bDoTeleportAnim; };
	struct __attribute__((packed)) { u8 _pad_baseAnimId[0x8CC]; byte baseAnimId; };
	struct __attribute__((packed)) { u8 _pad_unk8CD[0x8CD]; char unk8CD; };
	struct __attribute__((packed)) { u8 _pad_unk8CF[0x8CF]; byte unk8CF; };
	struct __attribute__((packed)) { u8 _pad_numEntriesField450[0x8D0]; byte numEntriesField450; };
	struct __attribute__((packed)) { u8 _pad_numEntriesField454[0x8D1]; byte numEntriesField454; };
	struct __attribute__((packed)) { u8 _pad_numEntriesField458[0x8D2]; byte numEntriesField458; };
	struct __attribute__((packed)) { u8 _pad_numEntriesField45C[0x8D3]; byte numEntriesField45C; };
	struct __attribute__((packed)) { u8 _pad_numEntriesField460[0x8D4]; byte numEntriesField460; };
	struct __attribute__((packed)) { u8 _pad_flags8D8[0x8D8]; ushort flags8D8; };
	struct __attribute__((packed)) { u8 _pad_unk8DB[0x8DB]; byte unk8DB; };
	struct __attribute__((packed)) { u8 _pad_curSeqBit[0x8DC]; GameBit16* curSeqBit; };
} ObjState_Player;
