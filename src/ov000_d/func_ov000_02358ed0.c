#include <ov000_d/scene.h>

void func_ov000_023598dc(s32 a);
BOOL func_ov000_023567d4(void);
void func_ov000_02359838(s32 a);
BOOL func_ov000_02356794(void);
BOOL func_ov000_02356754(void);
s32 func_ov000_02356570(UnkStruct_Scene *w);
BOOL func_ov000_02356728(void);
BOOL func_ov000_02356710(void);
BOOL func_ov000_02356590(UnkStruct_Scene *w);
void func_02051f20(void);
void func_ov000_02359744(s32 a);
void func_ov000_023597b8(s32 a);
void func_ov000_023596f4(s32 a);
void func_ov000_02357cf8(UnkStruct_Scene *w);
void func_ov000_02357c5c(UnkStruct_Scene *w);

#pragma thumb on

void func_ov000_02358ed0(void) {
    UnkStruct_Scene *w = data_022b4574->work;
    UnkStruct_ScenePos *p;
    u32 step;
    s32 i;
    s32 scale;
    s32 n;

    func_ov000_02356f7c(&w->mdl, 0);
    func_ov000_02356e2c(&w->mdl);
    func_ov000_02356d88(&w->mdl);
    func_ov000_02357ec8(w, w->unk14, 0x1000);
    func_ov000_023582e8(w, w->unk14, 0);
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
    func_ov000_02357970(w);
    if (w->unk0 != 0 || w->unk338 == 1) {
        p = &w->pos[5];
        func_ov000_023578d0(w, p->x, p->y);
        w->unk338 = 0;
    }
    if (w->unk10 == 2) {
        step = 0x8000;
    } else {
        step = 0x5555;
    }
    for (i = 1; i < w->unk10; i++) {
        func_ov000_02358384(w, (w->unk14 + i) % w->unk10, step * i);
    }
    if (w->unk334 == 0 || w->unk0 >= 4) {
        scale = 0x1000;
    } else {
        scale = (s32)(w->unk0 * 0x600) / 4 + 0xa00;
    }
    func_ov000_02357878(w, 0x80, 0x98, scale);
    func_ov000_02358224(w, 0, 0x1000);
    if (w->unk334 == 1 && w->unk0 < 5) {
        func_ov000_023580bc(w, FALSE);
    } else {
        data_02087d04[2].x++;
        func_ov000_023580bc(w, TRUE);
    }
    if ((u32)(w->unkC - 1) <= 1 && w->unk14 == w->unk24) {
        func_ov000_02357810(w, 0, 0);
    }
    w->unk0++;
    if ((u32)(w->unkC - 1) <= 1 && w->unk14 == w->unk24) {
        n = w->unk14;
        if (w->unk20) {
            if (++n >= w->unk10) {
                n = 0;
            }
        } else {
            if (--n < 0) {
                n = w->unk10 - 1;
            }
        }
        w->unk18 = n;
        func_ov000_023598dc(0);
        return;
    }
    if (func_ov000_023567d4()) {
        func_ov000_02359838(0);
        return;
    }
    if (func_ov000_02356794()) {
        n = w->unk14 - 1;
        if (n < 0) {
            n = w->unk10 - 1;
        }
        w->unk20 = 0;
        w->unk18 = n;
        func_ov000_023598dc(0);
        return;
    }
    if (func_ov000_02356754()) {
        n = w->unk14 + 1;
        if (n >= w->unk10) {
            n = 0;
        }
        w->unk20 = 1;
        w->unk18 = n;
        func_ov000_023598dc(0);
        return;
    }
    if ((u32)(w->unkC - 1) <= 1) {
        n = func_ov000_02356570(w);
        if (w->unk24 != n) {
            w->unk24 = n;
        }
    }
    if (w->unkC != 2) {
        if (func_ov000_02356728()) {
            w->unk33C = 1;
            func_02051f20();
            if (w->unkC == 1) {
                func_ov000_02359744(0);
            } else {
                func_ov000_023597b8(0);
            }
            return;
        }
        if (func_ov000_02356710()) {
            w->unk33C = 0;
            func_02051f20();
            if (w->unkC == 1) {
                func_ov000_02359744(0);
            } else {
                func_ov000_023597b8(0);
            }
            return;
        }
    } else {
        if (func_ov000_02356590(w)) {
            func_02051f20();
            func_ov000_023596f4(0);
            return;
        }
    }
    if (w->unk0 == 1) {
        func_ov000_02357cf8(w);
        func_ov000_02357c5c(w);
    }
}

#pragma thumb reset
