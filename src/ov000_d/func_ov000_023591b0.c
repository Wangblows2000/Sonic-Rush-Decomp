#include <ov000_d/scene.h>

void func_ov000_023599b0(s32 a);

#pragma thumb on

void func_ov000_023591b0(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *p;
    s32 ang;
    s32 x;
    s32 t;
    s32 step;
    s32 base;
    s32 i;

    ang = func_ov000_02355f0c(0, 0x4000, 0x10, w->unk0, 0x2000);
    if (w->unk20) {
        ang = -(0x4000 - ang);
        if (ang > 0) {
            ang = 0;
        }
    } else {
        ang = 0x4000 - ang;
        if (ang < 0) {
            ang = 0;
        }
    }
    func_ov000_02357090(&w->mdl, w->unk14);
    func_ov000_02356f7c(&w->mdl, 0);
    func_ov000_02356e7c(&w->mdl, ang);
    func_ov000_02356e2c(&w->mdl);
    func_ov000_02356d88(&w->mdl);
    p = &w->pos[0];
    func_ov000_023579fc(w, p->x, p->y);
    p = &w->pos[1];
    func_ov000_023579c8(w, p->x, p->y);
    p = &w->pos[2];
    func_ov000_0235793c(w, p->x, p->y);
    p = &w->pos[3];
    func_ov000_02357908(w, p->x, p->y);
    if (w->unk0 >= 6) {
        func_02040d14(&w->pos[4]);
        func_02040d14(&w->pos[5]);
        p = &w->pos[4];
        func_ov000_02357994(w, p->x, p->y);
        func_02040d14(&w->pos[6]);
        func_ov000_02357ec8(w, w->unk14, w->pos[6].unk4);
        func_02040d14(&w->pos[7]);
        x = w->pos[7].x;
        if (!w->unk20) {
            x = -x;
        }
        func_ov000_023582e8(w, w->unk14, x);
    }
    if (w->unk14 == 2 || w->unk1C == 2) {
        t = (s32)((w->unk0 - 6) << 12) / 10;
    } else {
        t = 0x1000;
    }
    func_ov000_02358224(w, 0, t);
    if (w->unk10 == 2) {
        step = 0x8000;
    } else {
        step = 0x5555;
    }
    base = func_ov000_02355f0c(0, step, 0x20, w->unk0 + 0x10, 0x2000);
    if (w->unk20) {
        base = -base;
    }
    for (i = 0; i < w->unk10; i++) {
        func_ov000_02358384(w, (w->unk1C + i) % w->unk10, base + step * i);
    }
    func_ov000_02358108(w, 0);
    if ((u32)(w->unkC - 1) <= 1 && w->unk14 == w->unk24) {
        func_ov000_02357810(w, 0, 0);
    }
    w->unk0++;
    if (w->unk0 >= 0x10) {
        func_ov000_02356e7c(&w->mdl, 0);
        func_ov000_02357ec8(w, w->unk14, 0x1000);
        func_ov000_023582e8(w, w->unk14, 0);
        func_ov000_023599b0(0);
    }
}

#pragma thumb reset
