//include/sfa/xml/Game/GameObject/Camera.xml
typedef union Camera {
	vec3s rotation;
	struct __attribute__((packed)) { u8 _pad_flags[0x6]; ObjInstance_Flags06 flags; };
	struct __attribute__((packed)) { u8 _pad_scale[0x8]; float scale; };
	struct __attribute__((packed)) { u8 _pad_pos[0xC]; vec3f pos; };
	struct __attribute__((packed)) { u8 _pad_prevPos[0x18]; vec3f prevPos; };
	struct __attribute__((packed)) { u8 _pad_vel[0x24]; vec3f vel; };
	struct __attribute__((packed)) { u8 _pad_pMatrix[0x30]; ObjInstance* pMatrix; };
	struct __attribute__((packed)) { u8 _pad_unk38[0x38]; float unk38; };
	struct __attribute__((packed)) { u8 _pad_unk40[0x40]; short* unk40; };
	struct __attribute__((packed)) { u8 _pad_prevPos44[0x44]; vec3f prevPos44; };
	struct __attribute__((packed)) { u8 _pad_prevRot[0x50]; vec3s prevRot; };
	struct __attribute__((packed)) { u8 _pad_unk74[0x74]; float unk74; };
	struct __attribute__((packed)) { u8 _pad_unkA2[0xA2]; byte unkA2; };
	struct __attribute__((packed)) { u8 _pad_focus[0xA4]; ObjInstance* focus; };
	struct __attribute__((packed)) { u8 _pad_pos_0xa8[0xA8]; vec3f pos_0xa8; };
	struct __attribute__((packed)) { u8 _pad_fov[0xB4]; float fov; };
	struct __attribute__((packed)) { u8 _pad_prevPos2[0xB8]; vec3f prevPos2; };
	struct __attribute__((packed)) { u8 _pad_dist_0xc4[0xC4]; float dist_0xc4; };
	struct __attribute__((packed)) { u8 _pad_unkC8[0xC8]; float unkC8; };
	struct __attribute__((packed)) { u8 _pad_unkCC[0xCC]; float unkCC; };
	struct __attribute__((packed)) { u8 _pad_unkD0[0xD0]; float unkD0; };
	struct __attribute__((packed)) { u8 _pad_unkD4[0xD4]; float unkD4; };
	struct __attribute__((packed)) { u8 _pad_dist_0xd8[0xD8]; float dist_0xd8; };
	struct __attribute__((packed)) { u8 _pad_focusOverridePos[0xDC]; vec3f focusOverridePos; };
	struct __attribute__((packed)) { u8 _pad_zoomTimer[0xF4]; float zoomTimer; };
	struct __attribute__((packed)) { u8 _pad_zoomSpeed[0xF8]; float zoomSpeed; };
	struct __attribute__((packed)) { u8 _pad_unk100[0x100]; vec3s unk100; };
	struct __attribute__((packed)) { u8 _pad_firstPersonRot[0x106]; vec3s firstPersonRot; };
	struct __attribute__((packed)) { u8 _pad_firstPersonPos[0x10C]; vec3f firstPersonPos; };
	struct __attribute__((packed)) { u8 _pad_fovFirstPerson[0x118]; float fovFirstPerson; };
	struct __attribute__((packed)) { u8 _pad_overrideTarget[0x11C]; ObjInstance* overrideTarget; };
	struct __attribute__((packed)) { u8 _pad_unk120[0x120]; undefined4 unk120; };
	struct __attribute__((packed)) { u8 _pad_target[0x124]; ObjInstance* target; };
	struct __attribute__((packed)) { u8 _pad_prevTarget[0x128]; ObjInstance* prevTarget; };
	struct __attribute__((packed)) { u8 _pad_unk12C[0x12C]; float unk12C; };
	struct __attribute__((packed)) { u8 _pad_unk130[0x130]; float unk130; };
	struct __attribute__((packed)) { u8 _pad_targetHealthFraction[0x134]; float targetHealthFraction; };
	struct __attribute__((packed)) { u8 _pad_targetType[0x138]; ObjInteractType targetType; };
	struct __attribute__((packed)) { u8 _pad_interpMode[0x139]; byte interpMode; };
	struct __attribute__((packed)) { u8 _pad_letterboxSize[0x13B]; char letterboxSize; };
	struct __attribute__((packed)) { u8 _pad_unk13C[0x13C]; byte unk13C; };
	struct __attribute__((packed)) { u8 _pad_doFocusOverride[0x13D]; byte doFocusOverride; };
	struct __attribute__((packed)) { u8 _pad_unk13E[0x13E]; byte unk13E; };
	struct __attribute__((packed)) { u8 _pad_rotFlags_0x13f[0x13F]; byte rotFlags_0x13f; };
	struct __attribute__((packed)) { u8 _pad_unk140[0x140]; byte unk140; };
	struct __attribute__((packed)) { u8 _pad_flags_0x141[0x141]; byte flags_0x141; };
	struct __attribute__((packed)) { u8 _pad_unk142[0x142]; byte unk142; };
	struct __attribute__((packed)) { u8 _pad_unk143[0x143]; byte unk143; };
} Camera;
//include/sfa/xml/Game/GameObject/Camera.xml
typedef union CameraParamsMode54 {
	ObjInstance* obj00;
	struct __attribute__((packed)) { u8 _pad_obj04[0x4]; ObjInstance* obj04; };
	struct __attribute__((packed)) { u8 _pad_obj08[0x8]; ObjInstance* obj08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; byte unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk0D[0xD]; char unk0D; };
	struct __attribute__((packed)) { u8 _pad_timer[0x10]; float timer; };
	struct __attribute__((packed)) { u8 _pad_vec14[0x14]; vec3f vec14; };
	struct __attribute__((packed)) { u8 _pad_rot[0x20]; vec3s rot; };
} CameraParamsMode54;
//include/sfa/xml/Game/GameObject/Camera.xml
typedef union CameraParamsMode55 {
	float timer;
	struct __attribute__((packed)) { u8 _pad_yPos[0x4]; float yPos; };
} CameraParamsMode55;
//include/sfa/xml/Game/GameObject/Camera.xml
typedef union CameraParamsShipBattle {
	float rotZ;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; float unk04; };
	struct __attribute__((packed)) { u8 _pad_rotX[0x8]; float rotX; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; float unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; float unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; float unk14; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; float unk18; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; float unk1C; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; float unk20; };
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; float unk24; };
	struct __attribute__((packed)) { u8 _pad_state[0x28]; byte state; };
} CameraParamsShipBattle;
//include/sfa/xml/Game/GameObject/Camera.xml
typedef union CameraParamsTestStrength {
	int unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; byte unk04; };
	struct __attribute__((packed)) { u8 _pad_x1[0x10]; float x1; };
	struct __attribute__((packed)) { u8 _pad_x2[0x14]; float x2; };
	struct __attribute__((packed)) { u8 _pad_y1[0x18]; float y1; };
	struct __attribute__((packed)) { u8 _pad_y2[0x1C]; float y2; };
	struct __attribute__((packed)) { u8 _pad_z1[0x20]; float z1; };
	struct __attribute__((packed)) { u8 _pad_z2[0x24]; float z2; };
	struct __attribute__((packed)) { u8 _pad_x3[0x2C]; float x3; };
	struct __attribute__((packed)) { u8 _pad_y3[0x34]; float y3; };
	struct __attribute__((packed)) { u8 _pad_z3[0x3C]; float z3; };
	struct __attribute__((packed)) { u8 _pad_fov[0x40]; float fov; };
	struct __attribute__((packed)) { u8 _pad_unk44[0x44]; float unk44; };
	struct __attribute__((packed)) { u8 _pad_distance[0x60]; float distance; };
} CameraParamsTestStrength;
//include/sfa/xml/Game/GameObject/Camera.xml
typedef union CameraParamsViewfinder {
	undefined4 unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; undefined4 unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; ushort unk08; };
} CameraParamsViewfinder;
//include/sfa/xml/Game/GameObject/Camera.xml
typedef union FirstPersonState {
	undefined4 unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; undefined4 unk04; };
	struct __attribute__((packed)) { u8 _pad_posX18[0x10]; float posX18; };
	struct __attribute__((packed)) { u8 _pad_prevX[0x14]; float prevX; };
	struct __attribute__((packed)) { u8 _pad_rotX18[0x18]; float rotX18; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; float unk1C; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; float unk20; };
	struct __attribute__((packed)) { u8 _pad_prevY[0x24]; float prevY; };
	struct __attribute__((packed)) { u8 _pad_unk28[0x28]; float unk28; };
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; float unk2C; };
	struct __attribute__((packed)) { u8 _pad_posZ30[0x30]; float posZ30; };
	struct __attribute__((packed)) { u8 _pad_prevZ[0x34]; float prevZ; };
	struct __attribute__((packed)) { u8 _pad_unk38[0x38]; float unk38; };
	struct __attribute__((packed)) { u8 _pad_rotx3C[0x3C]; float rotx3C; };
	struct __attribute__((packed)) { u8 _pad_unk40[0x40]; float unk40; };
	struct __attribute__((packed)) { u8 _pad_unk44[0x44]; float unk44; };
	struct __attribute__((packed)) { u8 _pad_unk48[0x48]; float unk48; };
	struct __attribute__((packed)) { u8 _pad_unk4C[0x4C]; float unk4C; };
	struct __attribute__((packed)) { u8 _pad_unk50[0x50]; float unk50; };
	struct __attribute__((packed)) { u8 _pad_unk54[0x54]; float unk54; };
	struct __attribute__((packed)) { u8 _pad_unk58[0x58]; float unk58; };
	struct __attribute__((packed)) { u8 _pad_unk5C[0x5C]; float unk5C; };
	struct __attribute__((packed)) { u8 _pad_curve_0x78[0x78]; undefined1 curve_0x78; };
	struct __attribute__((packed)) { u8 _pad_rotX[0xE0]; float rotX; };
	struct __attribute__((packed)) { u8 _pad_unkE4[0xE4]; float unkE4; };
	struct __attribute__((packed)) { u8 _pad_unkF8[0xF8]; undefined4 unkF8; };
	struct __attribute__((packed)) { u8 _pad_unkFC[0xFC]; float* unkFC; };
	struct __attribute__((packed)) { u8 _pad_unk100[0x100]; float* unk100; };
	struct __attribute__((packed)) { u8 _pad_unk104[0x104]; float* unk104; };
	struct __attribute__((packed)) { u8 _pad_unk108[0x108]; undefined4 unk108; };
	struct __attribute__((packed)) { u8 _pad_curveFn_0x10c[0x10C]; undefined4 curveFn_0x10c; };
	struct __attribute__((packed)) { u8 _pad_fPtr_0x110[0x110]; undefined4 fPtr_0x110; };
	struct __attribute__((packed)) { u8 _pad_unk114[0x114]; float unk114; };
	struct __attribute__((packed)) { u8 _pad_unk11C[0x11C]; float unk11C; };
	struct __attribute__((packed)) { u8 _pad_x[0x120]; float x; };
	struct __attribute__((packed)) { u8 _pad_unk124[0x124]; float unk124; };
	struct __attribute__((packed)) { u8 _pad_unk128[0x128]; float unk128; };
	struct __attribute__((packed)) { u8 _pad_haveViewFinder[0x12D]; byte haveViewFinder; };
	struct __attribute__((packed)) { u8 _pad_unk130[0x130]; float unk130; };
} FirstPersonState;
//include/sfa/xml/Game/GameObject/Camera.xml
typedef union ViewportStruct {
	int x1;
	struct __attribute__((packed)) { u8 _pad_y1[0x4]; int y1; };
	struct __attribute__((packed)) { u8 _pad_x2[0x8]; int x2; };
	struct __attribute__((packed)) { u8 _pad_y2[0xC]; int y2; };
	struct __attribute__((packed)) { u8 _pad_flags[0x10]; undefined4 flags; };
} ViewportStruct;
