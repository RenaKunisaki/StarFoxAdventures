//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u8
	AnimCurvCmdEnum_PlaySound = 0x6,
	AnimCurvCmdEnum_ShowDialogue = 0xE,
	AnimCurvCmdEnum_SubCmd0D = 0xD,
	AnimCurvCmdEnum_QueueAction = 0xB,
	AnimCurvCmdEnum_CameraAnim = 0x1,
	AnimCurvCmdEnum_PlayAnimation = 0x2,
} AnimCurvCmdEnum;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u32
	ObjMsg_CollectiblePickup = 0x7000B,
	ObjMsg_New_Name = 0x50001,
} ObjMsg;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u32
	ObjSendMsgFlags_UseDefNo = 0x4,
	ObjSendMsgFlags_DontSendToSelf = 0x1,
	ObjSendMsgFlags_SendToAnyObj = 0x2,
} ObjSendMsgFlags;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u8
	ObjSeqCmdEnum_ClearMapBit = 0x14,
	ObjSeqCmdEnum_LoadMapAssets = 0x27,
	ObjSeqCmdEnum_UnloadMapAssets = 0x28,
	ObjSeqCmdEnum_PlaySound = 0x4,
	ObjSeqCmdEnum_TrickyCmd26 = 0x26,
	ObjSeqCmdEnum_CamAct06 = 0x6,
	ObjSeqCmdEnum_LockLevel = 0x2A,
	ObjSeqCmdEnum_MapBitToggle = 0x22,
	ObjSeqCmdEnum_SetBit = 0x12,
	ObjSeqCmdEnum_SaveGameSetMapBits_1E = 0x1E,
	ObjSeqCmdEnum_GameBitCmd1D = 0x1D,
	ObjSeqCmdEnum_SetPlayerModel = 0x10,
	ObjSeqCmdEnum_GameBitCmd1C = 0x1C,
	ObjSeqCmdEnum_SubCmd08 = 0x8,
	ObjSeqCmdEnum_RestructForMovie = 0x2E,
	ObjSeqCmdEnum_ChangeMapLayer = 0x20,
	ObjSeqCmdEnum_SaveCharPos = 0x1F,
	ObjSeqCmdEnum_GameTextCmd2D = 0x2D,
	ObjSeqCmdEnum_SaveGameSetMapBit = 0x1A,
	ObjSeqCmdEnum_LoadTexture = 0x15,
	ObjSeqCmdEnum_GameTimerCmd2F = 0x2F,
	ObjSeqCmdEnum_TrickyTalkCmd11 = 0x11,
	ObjSeqCmdEnum_PlayerCmd01 = 0x1,
	ObjSeqCmdEnum_NOP = 0x5,
	ObjSeqCmdEnum_SeqVar0B = 0xB,
	ObjSeqCmdEnum_SaveGameClearMapBit = 0x1B,
	ObjSeqCmdEnum_EnvFx0A = 0xA,
	ObjSeqCmdEnum_SaveGameCmd23 = 0x23,
	ObjSeqCmdEnum_GameBitToggle = 0x21,
	ObjSeqCmdEnum_SetMapBit = 0x13,
	ObjSeqCmdEnum_DoObjSeq = 0xC,
	ObjSeqCmdEnum_GetLActions = 0xD,
	ObjSeqCmdEnum_SaveGameSetMapBits_18 = 0x18,
	ObjSeqCmdEnum_SetObj2AnimField0 = 0x2C,
	ObjSeqCmdEnum_UnlockLevel = 0x2B,
	ObjSeqCmdEnum_UnloadTexture = 0x16,
} ObjSeqCmdEnum;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u8
	ObjSeqCmdEnum2_SetEyeAndEyelid = 0x8,
	ObjSeqCmdEnum2_SetEye = 0xE,
	ObjSeqCmdEnum2_SetEyelid = 0xF,
	ObjSeqCmdEnum2_SetInputOverride = 0xD,
	ObjSeqCmdEnum2_Dummy04 = 0x4,
	ObjSeqCmdEnum2_ParticleEffect = 0x3,
	ObjSeqCmdEnum2_DoNothing = 0x7,
	ObjSeqCmdEnum2_WarpToMap = 0x6,
	ObjSeqCmdEnum2_LoadDll = 0x5,
	ObjSeqCmdEnum2_ClearGameBit = 0xC,
	ObjSeqCmdEnum2_PlaySong = 0x0,
	ObjSeqCmdEnum2_GetEnvFxAct = 0x2,
	ObjSeqCmdEnum2_SetGameBit = 0xB,
} ObjSeqCmdEnum2;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u32
	ObjSeqCond32_Bool_eq_0 = 0x5,
	ObjSeqCond32_Bool_eq_1 = 0x6,
	ObjSeqCond32_Global1_lt_1 = 0x9,
	ObjSeqCond32_Global3_eq_0 = 0x11,
	ObjSeqCond32_Global3_ne_0 = 0x10,
	ObjSeqCond32_IsDay = 0x3,
	ObjSeqCond32_Global1_gt_0 = 0xA,
	ObjSeqCond32_IsNight = 0x4,
	ObjSeqCond32_Var1_eq_0 = 0x7,
	ObjSeqCond32_TimerDisabled = 0xD,
	ObjSeqCond32_Field60_lt_1 = 0x1,
	ObjSeqCond32_Global2_gt_0 = 0xC,
	ObjSeqCond32_TimerEnabled = 0xE,
	ObjSeqCond32_Global2_lt_1 = 0xB,
	ObjSeqCond32_Always = 0x0,
	ObjSeqCond32_Field60_gt_0 = 0x2,
	ObjSeqCond32_Var1_ne_0 = 0x8,
} ObjSeqCond32;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u8
	ObjSeqEventType_B_Pressed = 0x13,
	ObjSeqEventType_A_Pressed = 0x12,
	ObjSeqEventType_FptrEC = 0x1B,
	ObjSeqEventType_TalkingToNpc = 0x1A,
} ObjSeqEventType;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u8
	ObjSeqExecState_WaitForCmd4 = 0x3,
	ObjSeqExecState_SkipToEnd = 0x2,
} ObjSeqExecState;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u8
	ObjSeqFlags05_OverrideCulling = 0x1,
	ObjSeqFlags05_DifferentDistance = 0x2,
} ObjSeqFlags05;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u32
	ObjSeqFnReturn_Stop = 0x4,
	ObjSeqFnReturn_Continue = 0x0,
} ObjSeqFnReturn;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u16
	ObjSeqItemFlags_CantMove = 0x1,
	ObjSeqItemFlags_ForceAngle0 = 0x4,
	ObjSeqItemFlags_CantRotate = 0x2,
	ObjSeqItemFlags_CanMakeSound = 0x20,
	ObjSeqItemFlags_Enabled = 0x4000,
	ObjSeqItemFlags_TakeCameraFocus = 0x2000,
	ObjSeqItemFlags_CamFocusIdx = 0xF00,
	ObjSeqItemFlags_DisableJoypad = 0x40,
	ObjSeqItemFlags_CamRelated10 = 0x10,
} ObjSeqItemFlags;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u8
	ObjSeqItemFlags8_DisableLetterbox = 0x10,
	ObjSeqItemFlags8_CantMove = 0x1,
	ObjSeqItemFlags8_ForceAngle0 = 0x4,
	ObjSeqItemFlags8_CantRotate = 0x2,
	ObjSeqItemFlags8_CanMakeSound = 0x20,
	ObjSeqItemFlags8_DisableJoypad = 0x40,
} ObjSeqItemFlags8;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u32
	ObjSeqOp_Op2 = 0x2,
	ObjSeqOp_OpA = 0xA,
	ObjSeqOp_Op4 = 0x4,
	ObjSeqOp_Op3 = 0x3,
	ObjSeqOp_Op5 = 0x5,
	ObjSeqOp_Jump = 0x1,
} ObjSeqOp;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u8
	ObjState_AnimatedObj_Cmd81_UseItem = 0x1,
} ObjState_AnimatedObj_Cmd81;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u8
	ObjState_AnimatedObj_CurStep_Start = 0x3,
	ObjState_AnimatedObj_CurStep_Stop = 0x0,
	ObjState_AnimatedObj_CurStep_Run = 0x1,
} ObjState_AnimatedObj_CurStep;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u8
	ObjState_AnimatedObj_Flags90_SetBoolTo1 = 0x4,
	ObjState_AnimatedObj_Flags90_SetBoolTo0 = 0x8,
	ObjState_AnimatedObj_Flags90_DisableFade = 0x10,
	ObjState_AnimatedObj_Flags90_Yield = 0x40,
	ObjState_AnimatedObj_Flags90_New_Name = 0x3,
	ObjState_AnimatedObj_Flags90_EnableFade = 0x20,
	ObjState_AnimatedObj_Flags90_SetVar1to0 = 0x2,
	ObjState_AnimatedObj_Flags90_SetVar1to1 = 0x1,
} ObjState_AnimatedObj_Flags90;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u8
	RomListObjLoadFlags_OnlyLoadIfNearPlayer = 0x4,
	RomListObjLoadFlags_AllocatedByCode = 0x8,
	RomListObjLoadFlags_isManualLoad = 0x2,
	RomListObjLoadFlags_loadForOtherMap = 0x10,
	RomListObjLoadFlags_isLevelObject = 0x1,
	RomListObjLoadFlags_isBlockObject = 0x20,
} RomListObjLoadFlags;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef enum { //type:u8
	SeqCurveFlags_DoAnimAndSound = 0x2,
	SeqCurveFlags_HasActions = 0x1,
	SeqCurveFlags_SkipMostCmds = 0x8,
	SeqCurveFlags_DidCmd3 = 0x4,
} SeqCurveFlags;
typedef ObjSeqFnReturn (*ObjSeqFn)(ObjInstance* param1, ObjInstance* param2, ObjState_AnimatedObj* param3);
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union AnimCurvCommand {
	AnimCurvCmdEnum cmd;
	struct __attribute__((packed)) { u8 _pad_time[0x1]; byte time; };
	struct __attribute__((packed)) { u8 _pad_param[0x2]; short param; };
} AnimCurvCommand;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union AnimCurvHeader {
	char signature;
	struct __attribute__((packed)) { u8 _pad_size[0x4]; s16 size; };
	struct __attribute__((packed)) { u8 _pad_nActions[0x6]; s16 nActions; };
} AnimCurvHeader;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union ObjSeqAction {
	s16 param;
	struct __attribute__((packed)) { u8 _pad_len[0x2]; int len; };
	struct __attribute__((packed)) { u8 _pad_op[0x3]; int op; };
} ObjSeqAction;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union ObjSeqCmd {
	byte flags;
	struct __attribute__((packed)) { u8 _pad_cmd[0x1]; ObjSeqCmdEnum cmd; };
	struct __attribute__((packed)) { u8 _pad_param1[0x2]; byte param1; };
	struct __attribute__((packed)) { u8 _pad_param2[0x3]; byte param2; };
} ObjSeqCmd;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union ObjSeqCmd2 {
	ObjInstance* obj;
	struct __attribute__((packed)) { u8 _pad_param[0x4]; word param; };
	struct __attribute__((packed)) { u8 _pad_cmd[0x6]; ObjSeqCmdEnum2 cmd; };
	struct __attribute__((packed)) { u8 _pad_unk07[0x7]; byte unk07; };
} ObjSeqCmd2;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union ObjSeqMsg {
	ObjMsg msg;
	struct __attribute__((packed)) { u8 _pad_from[0x4]; ObjInstance* from; };
	struct __attribute__((packed)) { u8 _pad_param[0x8]; void* param; };
} ObjSeqMsg;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union ObjSeqMsgQueue {
	int nMsgs;
	struct __attribute__((packed)) { u8 _pad_length[0x4]; int length; };
} ObjSeqMsgQueue;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union ObjSequence61 {
	struct __attribute__((packed)) { u8 _pad_gameBit_0x1e[0x1E]; GameBit16 gameBit_0x1e; };
	struct __attribute__((packed)) { u8 _pad_gameBit_0x2c[0x2C]; GameBit16 gameBit_0x2c; };
} ObjSequence61;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union ObjSequenceStruct_60 {
	struct __attribute__((packed)) { u8 _pad_unk1E[0x1E]; short unk1E; };
	struct __attribute__((packed)) { u8 _pad_gameBitId[0x20]; short gameBitId; };
} ObjSequenceStruct_60;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union SeqAttachedObj {
	ObjInstance* copyTo;
	struct __attribute__((packed)) { u8 _pad_copyFrom[0x4]; ObjInstance* copyFrom; };
} SeqAttachedObj;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union SeqBgCmd {
	short seqIdx;
	struct __attribute__((packed)) { u8 _pad_curveTime5E[0x2]; short curveTime5E; };
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; short unk04; };
} SeqBgCmd;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union SeqObjStruct1 {
	ObjInstance* objsRunningSeqs;
	struct __attribute__((packed)) { u8 _pad_resumeTime[0x4]; dword resumeTime; };
} SeqObjStruct1;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union SeqStruct28 {
	dword objSeqData_8030eca8;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; pointer unk04; };
} SeqStruct28;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union SeqStruct_34 {
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; int unk2C; };
	struct __attribute__((packed)) { u8 _pad_unk94[0x94]; int unk94; };
	struct __attribute__((packed)) { u8 _pad_unk98[0x98]; undefined4 unk98; };
} SeqStruct_34;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union SeqSubCmd0B {
	pointer cmds;
	struct __attribute__((packed)) { u8 _pad_nCmds[0x4]; word nCmds; };
	struct __attribute__((packed)) { u8 _pad_curveTime[0x6]; word curveTime; };
} SeqSubCmd0B;
//include/sfa/xml/Game/GameObject/ObjSeq.xml
typedef union SequenceStruct2 {
	u8 flags;
	struct __attribute__((packed)) { u8 _pad_state[0x1]; u8 state; };
	struct __attribute__((packed)) { u8 _pad_action[0x2]; byte action; };
	struct __attribute__((packed)) { u8 _pad_bucket[0x3]; u8 bucket; };
	struct __attribute__((packed)) { u8 _pad_objNo[0x4]; short objNo; };
	struct __attribute__((packed)) { u8 _pad_unk06[0x6]; byte unk06; };
} SequenceStruct2;
