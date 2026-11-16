#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u16 unk12;
    s32 unk14;
    s32 unk18;
} UnkStruct_0235b1a4_e;

typedef struct {
    u8 pad[0x1a0];
    UnkStruct_0235b1a4_e e[64];
    u8 pad2[0x9d4 - 0x1a0 - 64 * 0x1c];
    u8 count;
} UnkStruct_0235b1a4_w;

typedef struct {
    u8 pad[0x10];
    UnkStruct_0235b1a4_w *unk10;
} UnkStruct_0235b1a4;

extern UnkStruct_0235b1a4 *gRingManagerTask;
extern u32 gRandSeed;
BOOL func_ov000_02345758(s32 x, s32 y);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void RingManager_SpawnRing(s32 a, s32 b, s16 c, s16 d, u16 f) {
    UnkStruct_0235b1a4_w *w;
    UnkStruct_0235b1a4_e *e;
    u8 i;

    if (gRingManagerTask == NULL)
        return;
    w = gRingManagerTask->unk10;
    if (w->count >= 64)
        return;
    for (i = 0; i < 64; i++) {
        e = &w->e[i];
        if (e->unk10 == 0) {
            e->unk0 = a;
            e->unk4 = b;
            e->unk8 = c;
            e->unkA = d;
            e->unkC = 0;
            e->unkE = 0;
            e->unk12 = f;
            e->unk10 = 0x100;
            e->unk10 += Rand() & 0x1f;
            e->unk18 = 0;
            e->unk14 = 0;
            if (func_ov000_02345758(a, b))
                e->unk12 |= 0x8000;
            else
                e->unk12 &= ~0x8000;
            w->count++;
            return;
        }
    }
}
