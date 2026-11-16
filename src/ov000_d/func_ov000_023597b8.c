#include <ov000_d/scene.h>

void func_ov000_023588dc(void);

#pragma thumb on

void func_ov000_023597b8(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    if (w->unk33C) {
        func_ov000_02357aac(w);
    } else {
        func_ov000_02357ae8(w);
    }
    func_ov000_02357d20(w);
    func_ov000_02357c8c(w);
    if (w->unk33C) {
        func_ov000_02356694(0, 0x10, 0x10, 0x18);
    } else {
        func_ov000_02356694(0, 0x10, 0x10, 0);
        w->unk0 = 0x18;
    }
    w->unk334 = 0;
    t->unk8 = func_ov000_023588dc;
}

#pragma thumb reset
