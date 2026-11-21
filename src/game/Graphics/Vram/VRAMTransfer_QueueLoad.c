#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; u32 unk10; } UnkStruct_020337a4;
UnkStruct_020337a4 *VRAMTransfer_AllocJob(void);
UnkStruct_020337a4 *OS_GetArenaLo(u32 a);

void VRAMTransfer_QueueLoad(u32 a, u32 b, u32 c) {
    UnkStruct_020337a4 *s = VRAMTransfer_AllocJob();
    if (OS_GetArenaLo(0) != s) {
        s->unk8 = b;
        s->unkC = c;
        s->unk4 = 1;
        s->unk10 = a;
    }
}
