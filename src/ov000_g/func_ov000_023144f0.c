#include <ov000_b/stageobj.h>

typedef struct {
    s16 x;
    s16 y;
} UnkStruct_023144f0_P;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_023144f0_T;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x94 - 0x18];
    u8 unk94[0x48];
    u8 unkDC[0x48];
    u8 pad124[0x204 - 0x124];
    UnkStruct_023144f0_T *unk204;
} UnkStruct_023144f0;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_023144f0 *work;
} UnkStruct_023144f0_Task;

extern UnkStruct_023144f0_Task *gTaskCurrent;

void StageObj_Draw2DEx(s32 x, s32 y, u8 rot, void *w, void *a, void *b, s32 c);

void func_ov000_023144f0(void) {
    UnkStruct_023144f0_P pts[8] = {0};
    UnkStruct_023144f0 *w;
    u8 n = 1;
    u8 i;
    s16 sx;
    s16 sy;

    w = gTaskCurrent->work;
    if (w->unk204 != NULL) {
        sx = ((w->unk204->unk10 - w->unk10) / 8) >> 8;
        sy = ((w->unk204->unk14 - w->unk14) / 8) >> 8;
        for (i = 0; i < 8; i++) {
            pts[i].x = sx * i + (w->unk10 >> 8);
            pts[i].y = sy * i + (w->unk14 >> 8);
        }
        if (w->unkC != 0) {
            for (i = 1; i < 8; i++) {
                pts[i].x += ((i + (w->unkC << 1)) & 7) - 4;
                pts[i].y += ((i + (w->unkC << 1)) & 7) - 4;
            }
        }
        n = 8;
    } else {
        pts[0].x = w->unk10 >> 8;
        pts[0].y = w->unk14 >> 8;
    }
    for (i = 0; i < n; i++) {
        StageObj_Draw2DEx(pts[i].x << 8, pts[i].y << 8, 0, NULL, w->unk94, w->unkDC, 0);
    }
}
