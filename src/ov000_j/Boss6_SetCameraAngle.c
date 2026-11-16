#include <nitro/types.h>

typedef struct {
    u8 pad0[0x8e];
    u16 unk8E;
    u8 pad90[2];
    s16 unk92;
} UnkStruct_0236a48c;

typedef struct {
    u8 pad0[0xd5e];
    u16 unkD5E;
} UnkStruct_022e015c;

extern UnkStruct_0236a48c data_ov000_0236a48c;

void Boss6_SetCameraAngle(UnkStruct_022e015c *p, s32 b) {
    UnkStruct_0236a48c *g = &data_ov000_0236a48c;

    if (b)
        g->unk8E = 0x1c71;
    else
        g->unk8E = 0xe38f;
    if (p->unkD5E != 0)
        g->unk8E += 0x8000;
    g->unk92 = 0x200;
}
