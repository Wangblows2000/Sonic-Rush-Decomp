#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
} UnkStruct_02356e7c;

extern s16 data_0206e470[];

#define FX_MUL(a, b) ((s32)(((s64)(a) * (b) + 0x800) >> 12))
#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

#pragma thumb on

void func_ov000_02356e7c(UnkStruct_02356e7c *w, s32 angle) {
    s32 s;
    s32 c;

    s = SIN((u16)angle);
    w->unk0 = FX_MUL(s, -0x16000);
    w->unk4 = 0;
    c = COS((u16)angle);
    w->unk8 = FX_MUL(c, -0x16000);
    w->unk10 = 0;
    w->unk14 = angle;
    w->unk18 = 0;
    w->unkC = 0x1a00;
    w->unk1C = w->unk0 + FX_MUL(c, -0x5000);
    w->unk20 = 0;
    w->unk24 = w->unk8 - FX_MUL(s, -0x5000);
    w->unk1C -= w->unk0;
    w->unk24 -= w->unk8;
    w->unk4 += (-0x16000 - w->unk8) / 4;
    w->unk24 -= 0xa000;
}

#pragma thumb reset
