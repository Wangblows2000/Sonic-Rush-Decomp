#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 frames;
    u8 pad8[0x18];
} UnkSeq_02040ef0;

typedef struct {
    u8 pad0[4];
    u32 count;
    u32 offset;
} UnkHdr_02040ef0;

typedef struct {
    s16 x;
    s16 y;
    s32 unk4;
    s32 unk8;
    s16 unkc;
    u8 pade[2];
    u32 flags;
    u32 seq;
    s32 frame;
    s32 speed;
    s32 unk20;
    s32 unk24;
    UnkHdr_02040ef0 *hdr;
} UnkAnim_02040ef0;

void MIi_CpuClear32(u32 data, void *dst, u32 size);

#pragma thumb on
void func_02040ef0(UnkAnim_02040ef0 *a, UnkHdr_02040ef0 *hdr, u32 seq) {
    vu32 v = 0;
    MIi_CpuClear32(v, a, 0x2c);
    a->hdr = hdr;
    if (seq < a->hdr->count)
        a->seq = seq;
    a->speed = 0x1000;
    a->unk20 = -1;
    a->unk24 = -1;
    a->flags |= 0x20;
}
#pragma thumb reset
