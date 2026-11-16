#include <ov000_d/scene.h>

void func_ov000_02359624(void);

#pragma thumb on

void func_ov000_02359c0c(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = data_022b4574;
    }
    w = t->work;
    w->unk0 = 0;
    func_02040ec0(&w->posB68, 0);
    func_ov000_023582e8(w, w->unk14, 0x100);
    w->unk330 = -0x100;
    func_ov000_023581dc(w);
    t->unk8 = func_ov000_02359624;
}

#pragma thumb reset
