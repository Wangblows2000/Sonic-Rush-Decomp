#include <ov000_c/stageobj.h>

extern UnkStruct_Task *gTaskCurrent;
extern s16 data_0206e470[];

void StageObj_Draw3D(void *w, void *a);
void StageObj_Draw2DEx(s32 x, s32 y, s32 z, void *w, void *a, void *b, s32 c);

void CranePartED_Draw(void) {
    UnkStruct_StageObj *w;
    s32 x;
    s32 y;
    s32 r;

    w = gTaskCurrent->work;
    r = 0x7400;

    StageObj_Draw3D(w, w->unk128);
    y = w->unk14 - 0x200;
    if (!(w->unk5C & 1))
        r -= 0x300;
    x = w->unk10 - (s16)((r * data_0206e470[(((u16)w->unk8 & 0xffff) >> 4) * 2 + 1]) >> 12);
    StageObj_Draw2DEx(x, y - 0x1200, 0, w, &w->spr0, &w->spr1, 0);
    if (w->unk54 & 2)
        return;
    if (!(w->unk5C & 1))
        x = w->unk10 - (s16)(((r + 0x100) * data_0206e470[(((u16)w->unk8 & 0xffff) >> 4) * 2 + 1]) >> 12);
    for (;;) {
        y -= 0x800;
        if (y < (w->unk1EC - 0x18) << 8)
            return;
        StageObj_Draw2DEx(x, y, 0, 0, (u8 *)w + 0x99c, (u8 *)w + 0x9e4, 0);
    }
}
