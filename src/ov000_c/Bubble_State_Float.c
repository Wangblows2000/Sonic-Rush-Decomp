#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x210 - 0x20C];
    s32 unk210;
    u16 unk214;
    u8 unk216;
} UnkStruct_023024b0;

extern UnkStruct_022c4560 gGameState;
extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]

u32 Bubble_Unk_ov000_02302164(UnkStruct_023024b0 *w);

static inline u16 FrameRand(void) {
    return (u16)(((gGameState.unk20 << 16) * 0x196225 + 0x3c6ef35f) >> 16);
}

void Bubble_State_Float(UnkStruct_023024b0 *w) {
    s32 v;
    u32 r;

    w->unk210++;
    switch (w->base.unk1E0->unk2) {
    case 0xe8:
        if ((w->base.unk54 & 2) && w->unk210 >= 10)
            w->base.unk54 &= ~2;
        if (!(FrameRand() & 0x1f))
            w->base.unk2E += (s16)(0xf - (Rand() & 0x1e));
        v = w->base.unk30;
        if (v < 0)
            v = -v;
        if (v < 0xd0)
            w->base.unk30 -= 2;
        if (w->unk210 >= 300)
            w->base.unk30 -= 1;
        break;
    case 0xe9:
        w->base.unk2E = ((SIN_IDX((s32)((u16)(w->unk210 << 11) & 0xffff) >> 4) << 7) >> 12) / 3;
        break;
    case 0xea:
        w->base.unk2E = (SIN_IDX((s32)((u16)(w->unk210 << 12) & 0xffff) >> 4) << 6) >> 12;
        break;
    }
    if ((w->base.unk14 >> 8) - w->unk216 <= w->unk214 + 3) {
        w->base.unk30 = 0;
        w->base.unk54 |= 4;
        return;
    }
    r = Bubble_Unk_ov000_02302164(w);
    if (r & 2) {
        w->base.unk54 |= 4;
        return;
    }
    if (r & 5)
        w->base.unk2E = -w->base.unk2E;
}
