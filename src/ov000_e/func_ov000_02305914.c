#include <ov000_c/stageobj.h>

extern UnkStruct_Task *data_022b4574;
extern s16 data_0206e470[];

void func_0206aa04(void *w, void *a);
void func_0206b768(s32 x, s32 y, s32 z, void *w, void *a, void *b, s32 c);

void func_ov000_02305914(void) {
    UnkStruct_StageObj *w;
    s32 x;
    s32 y;
    s32 r;

    w = data_022b4574->work;
    r = 0x7400;

    func_0206aa04(w, w->unk128);
    y = w->unk14 - 0x200;
    if (!(w->unk5C & 1))
        r -= 0x300;
    x = w->unk10 - (s16)((r * data_0206e470[(((u16)w->unk8 & 0xffff) >> 4) * 2 + 1]) >> 12);
    func_0206b768(x, y - 0x1200, 0, w, &w->spr0, &w->spr1, 0);
    if (w->unk54 & 2)
        return;
    if (!(w->unk5C & 1))
        x = w->unk10 - (s16)(((r + 0x100) * data_0206e470[(((u16)w->unk8 & 0xffff) >> 4) * 2 + 1]) >> 12);
    for (;;) {
        y -= 0x800;
        if (y < (w->unk1EC - 0x18) << 8)
            return;
        func_0206b768(x, y, 0, 0, (u8 *)w + 0x99c, (u8 *)w + 0x9e4, 0);
    }
}
