#include <nitro/types.h>

typedef struct UnkNode_02034174 {
    struct UnkNode_02034174 *prev;
    struct UnkNode_02034174 *next;
    u8 unk8[0x5c];
} UnkNode_02034174;

extern u32 gFileRequestUsed;
extern UnkNode_02034174 *gFileRequestFreeList[];
extern UnkNode_02034174 gFileRequestPool[];
extern UnkNode_02034174 *gFileRequestListHead;
extern UnkNode_02034174 *gFileRequestListTail;

void func_0200be98(s32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
UnkNode_02034174 *FileRequest_AllocNode(void);

void FileRequest_Init(void) {
    s32 i;
    func_0200be98(-1);
    {
        vu32 zero = 0;
        gFileRequestUsed = 0;
        MIi_CpuClear32(zero, gFileRequestPool, sizeof(UnkNode_02034174) * 16);
    }
    for (i = 0; i < 16; i++) {
        gFileRequestFreeList[i] = &gFileRequestPool[i];
    }
    gFileRequestListHead = FileRequest_AllocNode();
    gFileRequestListTail = FileRequest_AllocNode();
    gFileRequestListHead->prev = NULL;
    gFileRequestListHead->next = gFileRequestListTail;
    gFileRequestListTail->prev = gFileRequestListHead;
    gFileRequestListTail->next = NULL;
}
