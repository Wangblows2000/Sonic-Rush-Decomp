#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 unk20C[0x48];
    u8 unk254[0x48];
    u8 unk29C[0x48];
    u8 unk2E4[0x48];
    s16 unk32C;
    s16 unk32E;
} UnkStruct_02304a1c;

extern UnkStruct_Task *data_022b4574;

void func_0206bd24(void *w, UnkStruct_Spr *a, UnkStruct_Spr *b);
s32 func_02002094(s32 a, s32 b);
void func_0206b768(s32 x, s32 y, s32 z, void *w, void *a, void *b, s32 c);

void func_ov000_02304a1c(void) {
    s16 pts[5][2] = {0};
    UnkStruct_02304a1c *w = data_022b4574->work;
    s16 dx;
    s16 dy;
    s16 i;
    s32 ax;
    s32 ay;

    func_0206bd24(w, &w->base.spr0, &w->base.spr1);
    dx = func_02002094((w->unk32C << 8) - w->base.unk10, 5) >> 8;
    dy = func_02002094((w->unk32E << 8) - w->base.unk14, 5) >> 8;
    i = 0;
    ax = i;
    ay = i;
    for (; i < 5; i++) {
        pts[i][0] = w->unk32C - ax;
        pts[i][1] = w->unk32E - ay;
        ax += dx;
        ay += dy;
    }
    for (i = 0; i < 5; i++)
        func_0206b768(pts[i][0] << 8, pts[i][1] << 8, 0, 0, w->unk20C, w->unk254, 0);
    func_0206b768(pts[0][0] << 8, pts[0][1] << 8, 0, w, w->unk29C, w->unk2E4, 0);
}
