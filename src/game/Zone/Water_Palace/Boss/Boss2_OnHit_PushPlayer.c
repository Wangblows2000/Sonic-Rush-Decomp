#include <nitro/types.h>

typedef struct UnkStruct_022cd738_3 {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022cd738_3;

typedef struct UnkStruct_022cd738_2 {
    u8 unk0;
    u8 pad[0x10 - 1];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    s16 unk2E;
    u8 pad1[0x34 - 0x30];
    s16 unk34;
    s16 unk36;
    u8 pad2[0x58 - 0x38];
    u32 unk58;
} UnkStruct_022cd738_2;

typedef struct UnkStruct_022cd738 {
    s32 unk0;
    u8 pad[8];
    s16 unkC;
    u8 pad0[2];
    s16 unk10;
    u8 pad1[6];
    void *unk18;
} UnkStruct_022cd738;

void Boss2_OnHit_PushPlayer(UnkStruct_022cd738 *a, UnkStruct_022cd738 *b) {
    UnkStruct_022cd738_2 *p = b->unk18;
    UnkStruct_022cd738_3 *o = a->unk18;
    s32 vel;
    s32 px;
    if (p->unk0 != 1)
        return;
    if (p->unk58 & 0x8000)
        vel = p->unk2E;
    else
        vel = p->unk34;
    px = p->unk10;
    if (a->unk0 < b->unk0) {
        if (vel > 0)
            return;
        if (o->unk5C & 1)
            p->unk36 = ((a->unk0 + a->unk10 - b->unkC) << 8) - px;
        else
            p->unk36 = ((a->unk0 - a->unkC - b->unkC) << 8) - px;
        p->unk58 |= 4;
        p->unk2E = 0;
        p->unk34 = 0;
    } else {
        if (vel < 0)
            return;
        if (o->unk5C & 1)
            p->unk36 = ((a->unk0 + a->unkC - b->unk10 + 2) << 8) - px;
        else
            p->unk36 = ((a->unk0 - a->unk10 - b->unk10 + 2) << 8) - px;
        p->unk58 |= 4;
        p->unk2E = 0;
        p->unk34 = 0;
    }
}
