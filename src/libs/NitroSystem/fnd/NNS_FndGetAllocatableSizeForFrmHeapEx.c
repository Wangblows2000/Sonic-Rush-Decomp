#include <nitro/types.h>

s32 abs(s32 x);

typedef struct { void *head; void *tail; } UnkSub_02020fb0;
typedef struct { u8 unk0[0x24]; UnkSub_02020fb0 unk24; } UnkStruct_02020fb0;

u32 NNS_FndGetAllocatableSizeForFrmHeapEx(UnkStruct_02020fb0 *s, s32 align) {
    const UnkSub_02020fb0 *hd;
    const void *block;
    align = abs(align);
    hd = &s->unk24;
    block = (void *)(((align - 1) + (u32)hd->head) & ~(align - 1));
    if ((u32)block > (u32)hd->tail) return 0;
    return (u32)hd->tail - (u32)block;
}
