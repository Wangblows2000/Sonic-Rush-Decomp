#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    UnkStruct_StageObj *work;
} UnkStruct_023116d0_Task;

extern UnkStruct_023116d0_Task *gTaskCurrent;

void StageObj_Draw2D(void *w, void *a, void *b);

void func_ov000_023116d0(void) {
    UnkStruct_StageObj *w = gTaskCurrent->work;
    s32 ox = w->unk10;
    s32 oy = w->unk14;
    u16 i;

    if (w->unk1E0->unk2 == 0xc0) {
        if (w->unk1E0->unk7 > 0)
            w->unk10 = (w->unk8 * 12 + w->unk1E8) << 8;
        else
            w->unk10 = (w->unk1E8 - w->unk8 * 12) << 8;
    } else {
        if (w->unk1E0->unk7 > 0)
            w->unk14 = (w->unk8 * 12 + w->unk1EC) << 8;
        else
            w->unk14 = (w->unk1EC - w->unk8 * 12) << 8;
    }
    for (i = 0; i < w->unk1E0->unk9 * 2; i++) {
        StageObj_Draw2D(w, &w->spr0, &w->spr1);
        if (w->unk1E0->unk2 == 0xc0) {
            if (w->unk1E0->unk7 > 0) {
                w->unk10 -= 0xc00;
                if (w->unk10 < w->unk1E8 << 8)
                    w->unk10 += (w->unk1E0->unk8 * 12) << 8;
            } else {
                w->unk10 += 0xc00;
                if (w->unk10 > w->unk1E8 << 8)
                    w->unk10 -= (w->unk1E0->unk8 * 12) << 8;
            }
        } else {
            if (w->unk1E0->unk7 > 0) {
                w->unk14 -= 0xc00;
                if (w->unk14 < w->unk1EC << 8)
                    w->unk14 += (w->unk1E0->unk8 * 12) << 8;
            } else {
                w->unk14 += 0xc00;
                if (w->unk14 > w->unk1EC << 8)
                    w->unk14 -= (w->unk1E0->unk8 * 12) << 8;
            }
        }
    }
    w->unk10 = ox;
    w->unk14 = oy;
}
