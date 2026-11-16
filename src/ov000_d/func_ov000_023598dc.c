#include <ov000_d/scene.h>

void func_ov000_02358cc8(void);

#pragma thumb on

void func_ov000_023598dc(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = data_022b4574;
    }
    w = t->work;
    w->unk0 = 0;
    func_02040ec0(&w->pos[4], 6);
    func_02040ec0(&w->pos[5], 8);
    func_02040ec0(&w->pos[6], 0xa);
    func_02040ec0(&w->pos[7], 0xc);
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
    func_ov000_02357aa8(w, 0);
    func_ov000_02357aa8(w, 1);
    func_ov000_02357aa8(w, 2);
    func_ov000_02358138(w);
    w->unk334 = 0;
    if (w->unkC == 0 || (u32)(w->unkC - 3) <= 1) {
        func_02052104(5);
    }
    t->unk8 = func_ov000_02358cc8;
}

#pragma thumb reset
