#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    s32 unk210;
    u16 unk214;
} UnkStruct_02302a28;

extern UnkStruct_022c4560 data_022c4560;

void func_ov000_02329f50(UnkStruct_02302a28 *w, s16 x, s16 y, s32 type, u16 c);
void BubbleSpawner_SpawnBubble(UnkStruct_02302a28 *w, s32 type, s32 x, s32 y);

static inline u16 FrameRand(void) {
    return (u16)(((data_022c4560.unk20 << 16) * 0x196225 + 0x3c6ef35f) >> 16);
}

void BubbleSpawner_State_Unk_ov000_02302a28(UnkStruct_02302a28 *w) {
    u32 f = w->unk20C;

    if ((f & 2) && !(f & 1))
        w->unk20C = f & ~2;
    else if (!(f & 2) && (f & 1))
        w->unk20C |= 2;
    w->unk20C &= ~1;
    if (w->unk210 < 0xb2) {
        if ((FrameRand() & 0x3f) == 0)
            func_ov000_02329f50(w, 7 - (Rand() & 0xe), 7 - (Rand() & 0xe), 0, w->unk214);
        if (!((FrameRand() & 0x1f) - 1))
            func_ov000_02329f50(w, 7 - (Rand() & 0xe), 7 - (Rand() & 0xe), 1, w->unk214);
    }
    w->unk210--;
    if (!(w->unk20C & 2))
        return;
    if (w->unk210 < 0) {
        w->unk210 = 0xd2;
        BubbleSpawner_SpawnBubble(w, 0xe8, w->base.unk10 + (0x3ff - (Rand() & 0x7fe)), w->base.unk14 + (0x3ff - (Rand() & 0x7fe)));
    }
}
