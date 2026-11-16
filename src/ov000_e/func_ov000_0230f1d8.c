#include <ov000_c/stageobj.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_0230f1d8_P;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_0230f1d8_P pts[16];
} UnkStruct_0230f1d8;

typedef struct {
    u8 pad[0x10];
    UnkStruct_0230f1d8 *unk10;
} UnkStruct_022b4574;

extern UnkStruct_022b4574 *data_022b4574;

void func_0206b768(s32 x, s32 y, s32 a, UnkStruct_0230f1d8 *w, UnkStruct_Spr *s0, UnkStruct_Spr *s1, s16 (*pts)[2]);

void func_ov000_0230f1d8(void) {
    s16 buf[16][2] = {0};
    u8 i;
    UnkStruct_0230f1d8 *w;

    w = data_022b4574->unk10;
    for (i = 0; i < 16; i++) {
        buf[i][0] = w->pts[i].x >> 8;
        buf[i][1] = w->pts[i].y >> 8;
    }
    func_0206b768(w->base.unk10, w->base.unk14, 0, w, &w->base.spr0, &w->base.spr1, buf);
}
