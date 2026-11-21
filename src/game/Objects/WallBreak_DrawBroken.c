#include <game/Stage/stageobj.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_0231b2d4_P;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_0231b2d4_P pts[16];
} UnkStruct_0231b2d4;

typedef struct {
    u8 pad[0x10];
    UnkStruct_0231b2d4 *unk10;
} UnkStruct_022b4574;

extern UnkStruct_022b4574 *gTaskCurrent;

void StageObj_Draw2DEx(s32 x, s32 y, s32 a, UnkStruct_0231b2d4 *w, UnkStruct_Spr *s0, UnkStruct_Spr *s1, s16 (*pts)[2]);

void WallBreak_DrawBroken(void) {
    s16 buf[16][2] = {0};
    u8 i;
    UnkStruct_0231b2d4 *w;

    w = gTaskCurrent->unk10;
    for (i = 0; i < 16; i++) {
        buf[i][0] = w->pts[i].x >> 8;
        buf[i][1] = w->pts[i].y >> 8;
    }
    StageObj_Draw2DEx(w->base.unk10, w->base.unk14, 0, w, &w->base.spr0, &w->base.spr1, buf);
}
