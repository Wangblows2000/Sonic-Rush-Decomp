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
    u8 *unk18;
} UnkStruct_0235b0c8_e;

typedef struct {
    u8 pad[0x1a0];
    UnkStruct_0235b0c8_e e[64];
    u8 pad2[0x9d4 - 0x1a0 - 64 * 0x1c];
    u8 count;
} UnkStruct_0235b0c8_w;

typedef struct {
    u8 pad[0x10];
    UnkStruct_0235b0c8_w *unk10;
} UnkStruct_0235b0c8;

extern UnkStruct_0235b0c8 *gRingManagerTask;
BOOL Stage_IsGravityFlipped(s32 x, s32 y);

void RingManager_PlaceRing(u8 *flag, s32 a, s32 b) {
    UnkStruct_0235b0c8_w *w;
    UnkStruct_0235b0c8_e *e;
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
            e->unk8 = 0;
            e->unkA = 0;
            e->unkC = 0;
            e->unkE = 0;
            e->unk12 = 0x620;
            e->unk10 = 0x80;
            *flag = 0xff;
            e->unk18 = flag;
            e->unk14 = 0;
            if (Stage_IsGravityFlipped(a, b))
                e->unk12 |= 0x8000;
            else
                e->unk12 &= ~0x8000;
            w->count++;
            return;
        }
    }
}
