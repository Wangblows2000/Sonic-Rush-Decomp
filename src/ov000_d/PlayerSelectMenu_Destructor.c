#include <ov000_d/scene.h>

void Touch_Disable(UnkStruct_SceneTask *t);
BOOL func_020513a8(void);
void PlayerSelectMenu_ReleaseModels(UnkStruct_Mdl *m);
void PlayerSelectMenu_ReleaseArchives(UnkStruct_Mdl *m);
void PlayerSelectMenu_ReleaseSprites(UnkStruct_Scene *w);
void func_ov000_02359cd4(UnkStruct_Scene *w);
void PlayerSelectMenu_FreeBuffers(UnkStruct_Scene *w);
void PlayerSelectMenu_ReleaseDisplay(void);

#pragma thumb on

void PlayerSelectMenu_Destructor(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w = t->work;

    Touch_Disable(t);
    if ((w->unkC == 1 || w->unkC == 2) && func_020513a8()) {
        PlayerSelectMenu_ReleaseModels(&w->mdl);
    } else {
        PlayerSelectMenu_ReleaseArchives(&w->mdl);
    }
    PlayerSelectMenu_ReleaseSprites(w);
    func_ov000_02359cd4(w);
    PlayerSelectMenu_FreeBuffers(w);
    PlayerSelectMenu_ReleaseDisplay();
}

#pragma thumb reset
