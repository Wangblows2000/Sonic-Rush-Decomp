#include <ov000_d/scene.h>

void func_ov000_02359a2c(s32 a);

#pragma thumb on

void func_ov000_02358cc8(void) {
    UnkStruct_Scene *w = data_022b4574->work;
    UnkStruct_ScenePos *p;
    s32 ang;
    s32 x;
    s32 step;
    s32 base;
    s32 i;
    s32 t;

    ang = func_ov000_02355f0c(0, 0x4000, 0x10, w->unk0, 0x2000);
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
    func_ov000_02356f7c(&w->mdl, 0);
    func_ov000_02356e7c(&w->mdl, ang);
    func_ov000_02356e2c(&w->mdl);
    func_ov000_02356d88(&w->mdl);
    func_02040d14(&w->pos[4]);
    func_02040d14(&w->pos[5]);
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
    func_02040d14(&w->pos[7]);
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
    base = func_ov000_02355f0c(0, step, 0x20, w->unk0, 0x2000);
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
            data_02087d04[2].x++;
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
        func_ov000_02359a2c(0);
    }
}

#pragma thumb reset
