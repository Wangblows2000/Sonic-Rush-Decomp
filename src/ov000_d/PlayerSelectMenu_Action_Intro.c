#include <ov000_d/scene.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_02359af4_Sync;

UnkStruct_02359af4_Sync *func_020514e4(void);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void PlayerSelectMenu_State_Intro(void);

#pragma thumb on

void PlayerSelectMenu_Action_Intro(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    w->unk20 = 1;
    w->unk18 = 4;
    KeyframeAnim_SetAnim(&w->pos[6], 9);
    KeyframeAnim_SetAnim(&w->pos[7], 0xb);
    KeyframeAnim_SetAnim(&w->pos[0], 1);
    KeyframeAnim_SetAnim(&w->pos[1], 2);
    KeyframeAnim_SetAnim(&w->pos[2], 3);
    KeyframeAnim_SetAnim(&w->pos[3], 4);
    KeyframeAnim_SetAnim(&w->pos[4], 5);
    KeyframeAnim_SetAnim(&w->pos[5], 7);
    func_ov000_02357d20(w);
    func_ov000_02357c8c(w);
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
    GX_SetGraphicsMode(1, 0, 1);
    *(vu16 *)0x400000e = (*(vu16 *)0x400000e & 0x43) | 0x69c;
    w->unk334 = 1;
    w->unk338 = 1;
    t->unk8 = PlayerSelectMenu_State_Intro;
    func_020514e4()->unk3 = 0;
}

#pragma thumb reset
