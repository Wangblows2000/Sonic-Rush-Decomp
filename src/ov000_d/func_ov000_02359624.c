#include <ov000_d/scene.h>

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
} UnkStruct_02359624_Pad;

extern UnkStruct_02359624_Pad gPad;

void func_ov000_02359af4(s32 a);

#pragma thumb on

void func_ov000_02359624(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *q = &w->posB68;
    s32 x;

    if (w->unkC != 1 && w->unkC != 2 && (gPad.trg & 1) && w->unk0 >= 0x15) {
        w->unk340 = 1;
    }
    if (w->unk340) {
        w->unk0 = 0x1e;
        func_02040e48(q);
    }
    func_02040d14(q);
    func_ov000_02358410(w, q->unk4);
    if (w->unk0 >= 0x26) {
        w->unk330 += 0x20;
    }
    func_ov000_02358108(w, w->unk330);
    x = (s32)(w->unk0 << 9) / 0x1e - 0x200;
    if (x > 0) {
        x = 0;
    }
    if (x < -0x100) {
        x = -0x100;
    }
    func_ov000_02358224(w, x, 0x1000);
    w->unk0++;
    if (func_02040cf4(q)) {
        func_ov000_02358410(w, 0x1000);
        func_ov000_02359af4(0);
    }
}

#pragma thumb reset
