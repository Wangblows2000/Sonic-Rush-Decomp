#include <ov000_d/scene.h>

void PlayerSelectMenu_State_ScrollOut(void);

#pragma thumb on

void PlayerSelectMenu_Action_ScrollOut(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    KeyframeAnim_SetAnim(&w->pos[4], 6);
    KeyframeAnim_SetAnim(&w->pos[5], 8);
    KeyframeAnim_SetAnim(&w->pos[6], 0xa);
    KeyframeAnim_SetAnim(&w->pos[7], 0xc);
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
        SysSound_PlaySE(5);
    }
    t->unk8 = PlayerSelectMenu_State_ScrollOut;
}

#pragma thumb reset
