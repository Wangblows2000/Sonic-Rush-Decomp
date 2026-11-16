#include <ov000_d/scene.h>

void Touch_Disable(UnkStruct_SceneTask *t);
BOOL func_020513a8(void);
void func_ov000_023570bc(UnkStruct_Mdl *m);
void func_ov000_02357308(UnkStruct_Mdl *m);
void func_ov000_02359cd8(UnkStruct_Scene *w);
void func_ov000_02359cd4(UnkStruct_Scene *w);
void func_ov000_02359c60(UnkStruct_Scene *w);
void func_ov000_0235604c(void);

#pragma thumb on

void PlayerSelectMenu_Destructor(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w = t->work;

    Touch_Disable(t);
    if ((w->unkC == 1 || w->unkC == 2) && func_020513a8()) {
        func_ov000_023570bc(&w->mdl);
    } else {
        func_ov000_02357308(&w->mdl);
    }
    func_ov000_02359cd8(w);
    func_ov000_02359cd4(w);
    func_ov000_02359c60(w);
    func_ov000_0235604c();
}

#pragma thumb reset
