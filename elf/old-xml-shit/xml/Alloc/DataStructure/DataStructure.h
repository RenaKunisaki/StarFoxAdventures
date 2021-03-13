//include/sfa/xml/Alloc/DataStructure.xml
typedef union RefCountedPtr {
	void* data;
	struct __attribute__((packed)) { u8 _pad_refCount[0x4]; byte refCount; };
} RefCountedPtr;
//include/sfa/xml/Alloc/DataStructure.xml
typedef union Stack {
	void* pData;
	struct __attribute__((packed)) { u8 _pad_pEnd[0x4]; void* pEnd; };
	struct __attribute__((packed)) { u8 _pad_itemSize[0xC]; short itemSize; };
	struct __attribute__((packed)) { u8 _pad_maxItems[0xE]; short maxItems; };
	struct __attribute__((packed)) { u8 _pad_numItems[0x10]; undefined2 numItems; };
	struct __attribute__((packed)) { u8 _pad_data[0x20]; int* data; };
} Stack;
//include/sfa/xml/Alloc/DataStructure.xml
typedef union StackNode {
	StackNode* next;
} StackNode;
//include/sfa/xml/Alloc/DataStructure.xml
typedef union struct_stbfStack {
	pointer unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; u8 unk04; };
} struct_stbfStack;
