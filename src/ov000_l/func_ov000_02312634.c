#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Spr spr[2];
} UnkWork_02312634;

void func_0206b768(s32 x, s32 y, u8 a, void *w, UnkStruct_Spr *s0, UnkStruct_Spr *s1, s32 b);

void func_ov000_02312634(UnkWork_02312634 *w) {
    s32 c;
    u8 n;
    u16 id;
    u8 i;
    s32 x;
    s32 y;
    s32 dy;

    c = w->base.unk1E0->unk6;
    id = w->base.unk1E0->unk2;
    if (c < 0)
        c = 0;
    n = c;
    if (id == 0xa0) {
        x = w->base.unk10;
        y = w->base.unk14 - 0x3c00;
        dy = -0x4000;
    } else {
        x = w->base.unk10;
        y = w->base.unk14 + 0x3c00;
        dy = 0x4000;
    }
    i = 0;
    for (; i < n; i++) {
        func_0206b768(x, y, w->base.unk1, w, &w->spr[0], &w->spr[1], 0);
        y += dy;
    }
}
