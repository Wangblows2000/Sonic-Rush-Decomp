#include <ov000_d/scene.h>

void PlayerSelectMenu_Action_ScrollIn(s32 a);

#pragma thumb on

void PlayerSelectMenu_State_ScrollOut(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *p;
    s32 ang;
    s32 x;
    s32 step;
    s32 base;
    s32 i;
    s32 t;

    ang = PlayerSelectMenu_EaseValue(0, 0x4000, 0x10, w->unk0, 0x2000);
    if (w->unk20) {
        if (ang > 0x4000) {
            ang = 0x4000;
        }
    } else {
        ang = -ang;
        if (ang < -0x4000) {
            ang = -0x4000;
        }
    }
    func_ov000_02357090(&w->mdl, w->unk14);
    PlayerSelectMenu_SetModelSelection(&w->mdl, 0);
    func_ov000_02356e7c(&w->mdl, ang);
    PlayerSelectMenu_AnimateModels(&w->mdl);
    PlayerSelectMenu_DrawModels(&w->mdl);
    KeyframeAnim_Update(&w->pos[4]);
    KeyframeAnim_Update(&w->pos[5]);
    p = &w->pos[0];
    func_ov000_023579fc(w, p->x, p->y);
    p = &w->pos[1];
    func_ov000_023579c8(w, p->x, p->y);
    p = &w->pos[2];
    func_ov000_0235793c(w, p->x, p->y);
    p = &w->pos[3];
    func_ov000_02357908(w, p->x, p->y);
    p = &w->pos[4];
    func_ov000_02357994(w, p->x, p->y);
    KeyframeAnim_Update(&w->pos[7]);
    x = w->pos[7].x;
    if (!w->unk20) {
        x = -x;
    }
    func_ov000_023582e8(w, w->unk14, x);
    if (w->unk10 == 2) {
        step = 0x8000;
    } else {
        step = 0x5555;
    }
    base = PlayerSelectMenu_EaseValue(0, step, 0x20, w->unk0, 0x2000);
    if (w->unk20) {
        base = -base;
    }
    for (i = 0; i < w->unk10; i++) {
        func_ov000_02358384(w, (w->unk14 + i) % w->unk10, base + step * i);
    }
    if (w->unk14 == 2 || w->unk18 == 2) {
        t = (s32)(w->unk0 << 12) / 10;
        if (t < 0x1000) {
            func_ov000_02358224(w, 0, 0x1000 - t);
        }
    } else {
        func_ov000_02358224(w, 0, 0x1000);
    }
    if (w->unk0 < 9) {
        if (w->unk0 < 5) {
            gDisplaySub[2].x++;
            func_ov000_02358070(w, TRUE);
        } else {
            func_ov000_02358070(w, FALSE);
        }
    } else {
        func_ov000_023581dc(w);
        func_ov000_02358108(w, 0);
    }
    if ((u32)(w->unkC - 1) <= 1 && w->unk14 == w->unk24) {
        func_ov000_02357810(w, 0, 0);
    }
    w->unk0++;
    if (w->unk0 >= 0x10) {
        PlayerSelectMenu_Action_ScrollIn(0);
    }
}

#pragma thumb reset
