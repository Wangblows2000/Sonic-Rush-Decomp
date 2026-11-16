#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_0231c9ac_P;

typedef struct {
    u8 pad0[0x5C];
    u16 unk5C;
    u8 pad5E[0x94 - 0x5E];
    u8 unk94[0x48];
    u8 unkDC[0x48];
    u8 pad124[0x20C - 0x124];
    u8 unk20C[0x48];
    u8 unk254[0x48];
    u8 unk29C[0x48];
    u8 unk2E4[0x48];
    u8 pad32C[0x330 - 0x32C];
    UnkStruct_0231c9ac_P pts[16];
    s16 cnt[16];
} UnkStruct_0231c9ac;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_0231c9ac *work;
} UnkStruct_0231c9ac_Task;

extern UnkStruct_0231c9ac_Task *gTaskCurrent;

void StageObj_Draw2D(void *w, void *a, void *b);
void StageObj_Draw2DEx(s32 x, s32 y, u8 rot, void *w, void *a, void *b, s32 c);

void func_ov000_0231c9ac(void) {
    UnkStruct_0231c9ac *w = gTaskCurrent->work;
    s16 i;

    StageObj_Draw2D(w, w->unk94, w->unkDC);
    for (i = 0; i < 16; i++) {
        if (i == 3) {
            if (!(w->unk5C & 0x20))
                StageObj_Draw2DEx(w->pts[i].x, w->pts[i].y, 0, NULL, w->unk29C, w->unk2E4, 0);
        } else if (w->pts[i].x != 0 && (i > 3 || !(w->unk5C & 0x20)) && (w->cnt[i] < 0x3c || (w->cnt[i] & 2))) {
            StageObj_Draw2DEx(w->pts[i].x, w->pts[i].y, 0, NULL, w->unk20C, w->unk254, 0);
        }
    }
}
