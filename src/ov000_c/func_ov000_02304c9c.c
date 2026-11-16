#include <ov000_c/stageobj.h>

extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void func_ov000_02304fa0(UnkStruct_StageObj *w);
s16 func_02068df8(s32 v, s32 a, s32 m);
s16 func_02068db4(s32 v, s32 a);

void func_ov000_02304c9c(UnkStruct_StageObj *w) {
    u32 f;
    u32 flip;
    s32 i, c, x, y;

    if (w->unk1F4 & 2) {
        func_ov000_02304fa0(w);
        return;
    }
    f = w->unk1E0->unk4;
    flip = w->unk1F4 & 4;
    if (flip)
        f ^= 1;
    w->unkC += w->unk34 >> 2;
    if (w->unk1F4 & 4) {
        if (f & 2)
            w->unk34 = func_02068df8(w->unk34, -0x30, 0xc00);
        else
            w->unk34 = func_02068df8(w->unk34, 0x30, 0xc00);
    } else if (f & 1) {
        w->unk34 = func_02068db4(w->unk34, 0xd);
        if (w->unk34 == 0)
            w->unk1F4 |= 1;
    }
    if (w->unk1F4 & 1) {
        if (f & 2) {
            w->unk34 = func_02068df8(w->unk34, 0x30, 0xc00);
            if (w->unkC > 0) {
                w->unkC = 0;
                w->unk1F4 |= 2;
                w->unk34 = -w->unk34;
            }
        } else {
            w->unk34 = func_02068df8(w->unk34, -0x30, 0xc00);
            if (w->unkC < 0) {
                w->unkC = 0;
                w->unk1F4 |= 2;
                w->unk34 = -w->unk34;
            }
        }
    } else {
        if (w->unkC > 0x8000) {
            w->unkC = 0x8000;
            w->unk1F4 |= 2;
            w->unk1F4 ^= 4;
        }
        if (w->unkC < -0x8000) {
            w->unkC = -0x8000;
            w->unk1F4 |= 2;
            w->unk1F4 ^= 4;
        }
    }
    i = (s32)((u16)w->unkC & 0xffff) >> 4;
    c = (COS_IDX(i) * -0xc000) >> 12;
    x = (w->unk1E8 << 8) + ((SIN_IDX(i) * 0xc000) >> 12);
    if (f & 1) {
        if (flip)
            y = (w->unk1EC << 8) + 0xc000 - c;
        else
            y = (w->unk1EC << 8) - 0xc000 - c;
    } else {
        if (flip)
            y = (w->unk1EC << 8) - 0xc000 + c;
        else
            y = (w->unk1EC << 8) + 0xc000 + c;
    }
    w->unk2E = x - w->unk10;
    w->unk30 = y - w->unk14;
}
