//include/sfa/xml/Game/GameObject/ObjState/AnimatedObj.xml
typedef enum { //type:u16
	ObjState_AnimatedObj_Flags6E_Eyes = 0x40,
	ObjState_AnimatedObj_Flags6E_Rot9E = 0x8,
	ObjState_AnimatedObj_Flags6E_Position = 0x1,
	ObjState_AnimatedObj_Flags6E_AnimTimer = 0x4,
	ObjState_AnimatedObj_Flags6E_Scale = 0x10,
	ObjState_AnimatedObj_Flags6E_Opacity = 0x20,
	ObjState_AnimatedObj_Flags6E_Rotation = 0x2,
} ObjState_AnimatedObj_Flags6E;
//include/sfa/xml/Game/GameObject/ObjState/AnimatedObj.xml
typedef enum { //type:u8
	ObjState_AnimatedObj_Flags136_IsSavePoint = 0x4,
} ObjState_AnimatedObj_Flags136;
//include/sfa/xml/Game/GameObject/ObjState/AnimatedObj.xml
typedef enum { //type:u8
	ObjState_AnimatedObj_Flags7F_DisableJoypad = 0x2,
} ObjState_AnimatedObj_Flags7F;
//include/sfa/xml/Game/GameObject/ObjState/AnimatedObj.xml
typedef union ObjState_AnimatedObj_AttrCurves {
	float posOffs;
	struct __attribute__((packed)) { u8 _pad_curveId[0x4]; ObjUniqueId curveId; };
	struct __attribute__((packed)) { u8 _pad_posOffsVec[0x1C]; vec3f posOffsVec; };
	struct __attribute__((packed)) { u8 _pad_posOffsScale[0x28]; float posOffsScale; };
	struct __attribute__((packed)) { u8 _pad_rotOffs[0x2C]; vec3s rotOffs; };
	struct __attribute__((packed)) { u8 _pad_offsType[0x32]; byte offsType; };
	struct __attribute__((packed)) { u8 _pad_seqIdx[0x33]; byte seqIdx; };
	struct __attribute__((packed)) { u8 _pad_curveTime[0x34]; word curveTime; };
	struct __attribute__((packed)) { u8 _pad_curveTime5A[0x36]; short curveTime5A; };
	struct __attribute__((packed)) { u8 _pad_attrCurveMaxTime[0x38]; short attrCurveMaxTime; };
	struct __attribute__((packed)) { u8 _pad_curveTime5E[0x3A]; short curveTime5E; };
	struct __attribute__((packed)) { u8 _pad_unk3C[0x3C]; short unk3C; };
	struct __attribute__((packed)) { u8 _pad_nActions[0x3E]; s16 nActions; };
	struct __attribute__((packed)) { u8 _pad_nPoints[0x40]; short nPoints; };
	struct __attribute__((packed)) { u8 _pad_iAction[0x42]; short iAction; };
	struct __attribute__((packed)) { u8 _pad_length[0x44]; ushort length; };
	struct __attribute__((packed)) { u8 _pad_gameBit6A[0x46]; GameBit16 gameBit6A; };
	struct __attribute__((packed)) { u8 _pad_animId[0x48]; short animId; };
	struct __attribute__((packed)) { u8 _pad_flags6E[0x4A]; ObjState_AnimatedObj_Flags6E flags6E; };
	struct __attribute__((packed)) { u8 _pad_flags4C[0x4C]; short flags4C; };
	struct __attribute__((packed)) { u8 _pad_unk50[0x50]; undefined4 unk50; };
	struct __attribute__((packed)) { u8 _pad_camFlag78[0x54]; char camFlag78; };
	struct __attribute__((packed)) { u8 _pad_unk55[0x55]; byte unk55; };
	struct __attribute__((packed)) { u8 _pad_bOnlyXZ[0x56]; char bOnlyXZ; };
	struct __attribute__((packed)) { u8 _pad_objIdx[0x57]; char objIdx; };
	struct __attribute__((packed)) { u8 _pad_conditionPlus1[0x58]; char conditionPlus1; };
	struct __attribute__((packed)) { u8 _pad_unk59[0x59]; byte unk59; };
	struct __attribute__((packed)) { u8 _pad_unk5A[0x5A]; byte unk5A; };
	struct __attribute__((packed)) { u8 _pad_flags7F[0x5B]; ObjState_AnimatedObj_Flags7F flags7F; };
	struct __attribute__((packed)) { u8 _pad_unk5C[0x5C]; GameBit16 unk5C; };
	struct __attribute__((packed)) { u8 _pad_nCurveIds[0x67]; byte nCurveIds; };
	struct __attribute__((packed)) { u8 _pad_animTimer[0x68]; byte animTimer; };
	struct __attribute__((packed)) { u8 _pad_eyeState[0x69]; byte eyeState; };
	struct __attribute__((packed)) { u8 _pad_eyelidState[0x6A]; byte eyelidState; };
	struct __attribute__((packed)) { u8 _pad_flags90[0x6C]; ObjState_AnimatedObj_Flags90 flags90; };
	struct __attribute__((packed)) { u8 _pad_pActions[0x70]; AnimCurvCommand* pActions; };
	struct __attribute__((packed)) { u8 _pad_curvePoints[0x74]; AnimCurvPoint* curvePoints; };
	struct __attribute__((packed)) { u8 _pad_attrFirstPoint[0x78]; ObJSeqCurveIdxs attrFirstPoint; };
	struct __attribute__((packed)) { u8 _pad_attrNumPoints[0x9E]; ObjSeqCurvePositions attrNumPoints; };
} ObjState_AnimatedObj_AttrCurves;
//include/sfa/xml/Game/GameObject/ObjState/AnimatedObj.xml
typedef union ObjState_AnimatedObj_Struct12C {
	byte unk00;
	struct __attribute__((packed)) { u8 _pad_unk01[0x1]; byte unk01; };
	struct __attribute__((packed)) { u8 _pad_unk02[0x2]; byte unk02; };
	struct __attribute__((packed)) { u8 _pad_unk03[0x3]; byte unk03; };
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; byte unk04; };
} ObjState_AnimatedObj_Struct12C;
//include/sfa/xml/Game/GameObject/ObjState/AnimatedObj.xml
typedef union ObjState_AnimatedObj {
	ObjInstance* obj;
	struct __attribute__((packed)) { u8 _pad_posOffsV[0x4]; vec3f posOffsV; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; float unk10; };
	struct __attribute__((packed)) { u8 _pad_rotOffsV[0x14]; vec3s rotOffsV; };
	struct __attribute__((packed)) { u8 _pad_rotXorZ[0x1A]; short rotXorZ; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; float unk20; };
	struct __attribute__((packed)) { u8 _pad_posOffs[0x24]; float posOffs; };
	struct __attribute__((packed)) { u8 _pad_curveId0[0x28]; ObjUniqueId curveId0; };
	struct __attribute__((packed)) { u8 _pad_curveId1[0x2C]; pointer curveId1; };
	struct __attribute__((packed)) { u8 _pad_soundVar30[0x30]; short soundVar30; };
	struct __attribute__((packed)) { u8 _pad_unk32[0x32]; short unk32; };
	struct __attribute__((packed)) { u8 _pad_unk34[0x34]; short unk34; };
	struct __attribute__((packed)) { u8 _pad_soundVar36[0x36]; short soundVar36; };
	struct __attribute__((packed)) { u8 _pad_soundId38[0x38]; SoundId soundId38; };
	struct __attribute__((packed)) { u8 _pad_soundId[0x3E]; SoundId soundId; };
	struct __attribute__((packed)) { u8 _pad_posOffsVec[0x40]; vec3f posOffsVec; };
	struct __attribute__((packed)) { u8 _pad_posOffsScale[0x4C]; float posOffsScale; };
	struct __attribute__((packed)) { u8 _pad_rotOffs[0x50]; vec3s rotOffs; };
	struct __attribute__((packed)) { u8 _pad_offsType[0x56]; byte offsType; };
	struct __attribute__((packed)) { u8 _pad_seqIdx[0x57]; byte seqIdx; };
	struct __attribute__((packed)) { u8 _pad_curveTime[0x58]; word curveTime; };
	struct __attribute__((packed)) { u8 _pad_prevCurveTime[0x5A]; short prevCurveTime; };
	struct __attribute__((packed)) { u8 _pad_curveLen[0x5C]; short curveLen; };
	struct __attribute__((packed)) { u8 _pad_curveTime5E[0x5E]; short curveTime5E; };
	struct __attribute__((packed)) { u8 _pad_var60[0x60]; short var60; };
	struct __attribute__((packed)) { u8 _pad_nActions[0x62]; s16 nActions; };
	struct __attribute__((packed)) { u8 _pad_nPoints[0x64]; short nPoints; };
	struct __attribute__((packed)) { u8 _pad_iAction[0x66]; short iAction; };
	struct __attribute__((packed)) { u8 _pad_nextCmdTime[0x68]; ushort nextCmdTime; };
	struct __attribute__((packed)) { u8 _pad_gameBit6A[0x6A]; GameBit16 gameBit6A; };
	struct __attribute__((packed)) { u8 _pad_animId[0x6C]; short animId; };
	struct __attribute__((packed)) { u8 _pad_enabledCurves[0x6E]; ObjState_AnimatedObj_Flags6E enabledCurves; };
	struct __attribute__((packed)) { u8 _pad_enabledCurves4C[0x70]; ObjState_AnimatedObj_Flags6E enabledCurves4C; };
	struct __attribute__((packed)) { u8 _pad_nextTime[0x74]; undefined4 nextTime; };
	struct __attribute__((packed)) { u8 _pad_camFlag78[0x78]; char camFlag78; };
	struct __attribute__((packed)) { u8 _pad_bHaveObj[0x79]; byte bHaveObj; };
	struct __attribute__((packed)) { u8 _pad_bOnlyXZ[0x7A]; char bOnlyXZ; };
	struct __attribute__((packed)) { u8 _pad_objIdx[0x7B]; char objIdx; };
	struct __attribute__((packed)) { u8 _pad_pauseUntilCondPlus1[0x7C]; char pauseUntilCondPlus1; };
	struct __attribute__((packed)) { u8 _pad_unk7D[0x7D]; byte unk7D; };
	struct __attribute__((packed)) { u8 _pad_step[0x7E]; ObjState_AnimatedObj_CurStep step; };
	struct __attribute__((packed)) { u8 _pad_flags7F[0x7F]; ObjState_AnimatedObj_Flags7F flags7F; };
	struct __attribute__((packed)) { u8 _pad_lastObjCmd[0x80]; byte lastObjCmd; };
	struct __attribute__((packed)) { u8 _pad_objCmds[0x81]; byte objCmds; };
	struct __attribute__((packed)) { u8 _pad_nObjCmds[0x8B]; byte nObjCmds; };
	struct __attribute__((packed)) { u8 _pad_animTimer[0x8C]; byte animTimer; };
	struct __attribute__((packed)) { u8 _pad_eyeState[0x8D]; byte eyeState; };
	struct __attribute__((packed)) { u8 _pad_eyelidState[0x8E]; byte eyelidState; };
	struct __attribute__((packed)) { u8 _pad_flags90[0x90]; ObjState_AnimatedObj_Flags90 flags90; };
	struct __attribute__((packed)) { u8 _pad_pActions[0x94]; AnimCurvCommand* pActions; };
	struct __attribute__((packed)) { u8 _pad_curvePoints[0x98]; AnimCurvPoint* curvePoints; };
	struct __attribute__((packed)) { u8 _pad_attrFirstPoint[0x9C]; ObJSeqCurveIdxs attrFirstPoint; };
	struct __attribute__((packed)) { u8 _pad_attrNumPoints[0xC2]; ObjSeqCurvePositions attrNumPoints; };
	struct __attribute__((packed)) { u8 _pad_onFree[0xE8]; undefined* onFree; };
	struct __attribute__((packed)) { u8 _pad_testEvent[0xEC]; undefined* testEvent; };
	struct __attribute__((packed)) { u8 _pad_unkF0[0xF0]; vec3f unkF0; };
	struct __attribute__((packed)) { u8 _pad_uniqueId[0x10C]; ObjUniqueId uniqueId; };
	struct __attribute__((packed)) { u8 _pad_target[0x110]; ObjInstance* target; };
	struct __attribute__((packed)) { u8 _pad_rot9EOffs_Y[0x114]; undefined2 rot9EOffs_Y; };
	struct __attribute__((packed)) { u8 _pad_rot9EOffs_X[0x116]; undefined2 rot9EOffs_X; };
	struct __attribute__((packed)) { u8 _pad_eventPositions[0x118]; short eventPositions; };
	struct __attribute__((packed)) { u8 _pad_eventTypes[0x12C]; ObjSeqEventType eventTypes; };
	struct __attribute__((packed)) { u8 _pad_unk136[0x136]; ObjState_AnimatedObj_Flags136 unk136; };
} ObjState_AnimatedObj;
