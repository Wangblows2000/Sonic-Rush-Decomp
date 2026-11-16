#include <ov000_d/scene.h>

void func_ov000_02356524(UnkStruct_Scene *w, s32 sel);
void func_ov000_02358a18(void);

#pragma thumb on

void func_ov000_02359838(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    w->unk4 = 0;
    func_ov000_02356f7c(&w->mdl, 1);
    func_ov000_02357b9c(w);
    func_ov000_02357d20(w);
    func_ov000_02357c8c(w);
    func_ov000_02357a30(w, w->unk14);
    if (!(w->unkC != 1 && w->unkC != 2)) {
        func_ov000_02356524(w, w->unk14);
    }
    if (w->unkC != 1 && w->unkC != 2) {
        switch (w->unk14) {
        case 0:
            func_020520e0(2);
            break;
        case 1:
            func_020520e0(3);
            break;
        case 2:
            func_020520e0(4);
            func_020520ac(5);
            break;
        }
    }
    w->unk334 = 0;
    w->unk8 = 0;
    t->unk8 = func_ov000_02358a18;
}

#pragma thumb reset
