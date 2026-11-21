#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[4];
    s16 unkC;
    s16 unkE;
    u8 pad10[2];
    u16 unk12;
} UnkStruct_0235ab48_A;

typedef struct {
    u8 pad0[0x14];
    u8 unk14[0x5C - 0x14];
    u8 unk5C[0x8A0 - 0x5C];
    u8 unk8A0[0x8E4 - 0x8A0];
    s16 unk8E4;
    s16 unk8E6;
    u8 pad8E8[0x8FC - 0x8E8];
    u16 unk8FC;
} UnkStruct_0235ab48;

void StageObj_Draw2DEx(s32 a, s32 b, s32 c, void *d, void *e, void *f, s32 g);

void RingManager_DrawRing2D(UnkStruct_0235ab48 *w, UnkStruct_0235ab48_A *a) {
    w->unk8E4 = a->unkC;
    w->unk8E6 = a->unkE;
    w->unk8FC &= ~2;
    if (a->unk12 & 0x8000) {
        w->unk8FC |= 2;
    }
    StageObj_Draw2DEx(a->unk0, a->unk4, 0, w->unk8A0, w->unk14, w->unk5C, 0);
}
