#include <ov000_d/scene.h>

void PlayerSelectMenu_Main(void);

#pragma thumb on

void func_ov000_02359c0c(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    func_02040ec0(&w->posB68, 0);
    func_ov000_023582e8(w, w->unk14, 0x100);
    w->unk330 = -0x100;
    func_ov000_023581dc(w);
    t->unk8 = PlayerSelectMenu_Main;
}

#pragma thumb reset
