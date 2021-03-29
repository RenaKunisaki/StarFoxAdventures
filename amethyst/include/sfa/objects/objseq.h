typedef enum { //type:u32
	ObjSendMsgFlags_DontSendToSelf = 0x1,
	ObjSendMsgFlags_SendToAnyObj   = 0x2,
    ObjSendMsgFlags_UseDefNo       = 0x4,
} ObjSendMsgFlags;

typedef enum { //type:u32
	ObjMsg_CollectiblePickup = 0x7000B,
} ObjMsg;

typedef struct PACKED ObjSeqMsg {
	ObjMsg msg;
	ObjInstance* from;
	void* param;
} ObjSeqMsg;

typedef struct PACKED ObjSeqMsgQueue {
	int nMsgs;
	int length;
    ObjSeqMsg msg[0]; //XXX verify
} ObjSeqMsgQueue;

typedef struct PACKED ObjSeqCmd {
	byte flags;
	u8   cmd;    //ObjSeqCmdEnum
	byte param1;
	byte param2;
} ObjSeqCmd;

typedef enum { //type:u32
	ObjSeqFnReturn_Stop = 0x4,
	ObjSeqFnReturn_Continue = 0x0,
} ObjSeqFnReturn;

typedef ObjSeqFnReturn (*ObjSeqFn)(
    ObjInstance* param1,
    ObjInstance* param2,
    void* param3 //ObjState_AnimatedObj*
);
