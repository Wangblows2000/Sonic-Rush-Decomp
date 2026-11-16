#include <ov000_d/scene.h>

void func_ov000_023591b0(void);

#pragma thumb on

void func_ov000_02359a2c(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = data_022b4574;
    }
    w = t->work;
    w->unk0 = 0;
    w->unk1C = w->unk14;
    w->unk14 = w->unk18;
    w->unk18 = 4;
    func_02040ec0(&w->pos[4], 5);
    func_02040ec0(&w->pos[5], 7);
    func_02040ec0(&w->pos[6], 9);
    func_02040ec0(&w->pos[7], 0xb);
    if (w->unk20) {
        func_ov000_02357d20(w);
        func_ov000_02357c2c(w);
    } else {
        func_ov000_02357cd0(w);
        func_ov000_02357c8c(w);
    }
    func_ov000_02357b4c(w);
    func_ov000_02357b24(w);
    func_ov000_02357be4(w);
    func_ov000_02357ae8(w);
    func_ov000_02356810(w);
    if (w->unk14 != 2) {
        func_ov000_023582b0(w);
    } else {
        func_ov000_02358278(w);
    }
    w->unk334 = 1;
    t->unk8 = func_ov000_023591b0;
}

#pragma thumb reset
