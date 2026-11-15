#include <nitro/types.h>

typedef struct { u16 elemSize; u16 unk2; u8 data[1]; } UnkSub_020261b8;
typedef struct { u8 unk0[0xC]; u8 unkC[6]; u16 unk12; } UnkStruct_020261b8;

void *func_020261b8(UnkStruct_020261b8 *p, u32 idx) {
    UnkSub_020261b8 *sub = (UnkSub_020261b8 *)(p->unkC + p->unk12);
    return sub->data + sub->elemSize * idx;
}
