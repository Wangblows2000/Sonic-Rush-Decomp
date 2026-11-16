#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    s16 unk34;
} UnkStruct_02329888_P;

typedef struct {
    u8 pad0[8];
    u32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
    u8 pad20[0x30 - 0x20];
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u8 pad58[0x88 - 0x58];
    UnkStruct_02329888_P *unk88;
} UnkStruct_02329888;

extern s16 data_0206e470[];

#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

u32 FX_DivS32(u32 a, u32 b);
s32 func_02066a9c(s32 x, s32 y, s32 a, s32 b, s32 c, s32 d);

void func_ov000_02329888(UnkStruct_02329888 *w) {
    s32 r;

    if (w->unk88 != NULL)
        w->unk34 = w->unk88->unk34 << 3;
    w->unkC += w->unk34;
    w->unk10 = w->unk1C + 0x5400 - ((COS((u16)((u8)FX_DivS32(w->unkC << 8, 0x21e52) << 8)) * 0x54) >> 4);
    w->unk14 += w->unk30;
    r = func_02066a9c(w->unk10 >> 8, w->unk14 >> 8, 1, 2, 0, 0);
    if (w->unk8 > (u32)(w->unk14 >> 8) || r < 0)
        w->unk54 |= 4;
}
