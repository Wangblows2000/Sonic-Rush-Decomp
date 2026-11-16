#include <ov000_d/scene.h>

void func_ov000_0235878c(void);

#pragma thumb on

void func_ov000_02359744(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = data_022b4574;
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
    func_02050078(9, 3, 1, 0, 1, 0, 0xc, 0x100010);
    w->unk334 = 0;
    t->unk8 = func_ov000_0235878c;
}

#pragma thumb reset
