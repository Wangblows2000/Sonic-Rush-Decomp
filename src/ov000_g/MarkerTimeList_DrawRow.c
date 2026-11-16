#include <ov000_b/stageobj.h>

void StageObj_Draw2DEx(s32 x, s32 y, u8 rot, void *w, void *a, void *b, s32 c);
s32 Hud_Unk_ov000_02337650(s32 a);

void MarkerTimeList_DrawRow(UnkStruct_StageObj *w, s32 x, s32 y, u32 digits, u16 k) {
    s16 i;
    s32 s0 = w->spr0.unk34;
    s32 s1 = w->spr1.unk34;
    s32 off = 0;
    u32 sh;
    u32 d;

    w->spr0.unk34 = s0;
    w->spr1.unk34 = s1;
    w->spr0.unk34 += (k + 1) << 7;
    w->spr1.unk34 += (k + 1) << 7;
    StageObj_Draw2DEx(x, y, 0, w, &w->spr0, &w->spr1, 0);
    off -= 0x4600;
    for (i = 0, sh = 0; i < 5; i++, sh += 4) {
        w->spr0.unk34 = s0;
        w->spr1.unk34 = s1;
        d = (digits & (0xf << sh)) >> sh;
        w->spr0.unk34 += d << 7;
        w->spr1.unk34 += d << 7;
        StageObj_Draw2DEx(x - off, y, 0, w, &w->spr0, &w->spr1, 0);
        off += 0xa00;
        if (i == 1)
            off += 0x600;
        if (i == 3)
            off += 0x600;
    }
    w->spr0.unk34 = Hud_Unk_ov000_02337650(0);
    w->spr1.unk34 = Hud_Unk_ov000_02337650(1);
    w->spr0.unk34 += 0xba0;
    w->spr1.unk34 += 0xba0;
    for (i = 0, off = -0x3200; i < 2; i++, off += 0x1a00) {
        StageObj_Draw2DEx(x - off, y, 0, w, &w->spr0, &w->spr1, 0);
    }
    w->spr0.unk34 = s0;
    w->spr1.unk34 = s1;
}
