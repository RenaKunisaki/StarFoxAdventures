typedef struct PACKED ZlbHeader {
	union {
        char str[4]; //"ZLB\0"
        u32  word;
    } signature;
	u32  version; //1 or "DIR\0"
	u32  decLen;  //output length
	u32  compLen; //compressed length
} ZlbHeader;
