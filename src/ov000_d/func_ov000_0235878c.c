#include <ov000_d/scene.h>

BOOL func_02050018(void);
BOOL func_0205000c(void);
void func_02051628(s32 a);
void func_ov000_0235650c(UnkStruct_Scene *w);
void func_ov000_023597b8(s32 a);
void func_ov000_023599b0(s32 a);

#pragma thumb on

void func_ov000_0235878c(void) {
    UnkStruct_Scene *w = data_022b4574->work;
    UnkStruct_ScenePos *p;
    u32 step;
    s32 i;

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
    p = &w->pos[5];
    func_ov000_023578d0(w, p->x, p->y);
    if (w->unk10 == 2) {
        step = 0x8000;
    } else {
        step = 0x5555;
    }
    for (i = 1; i < w->unk10; i++) {
        func_ov000_02358384(w, (w->unk14 + i) % w->unk10, step * i);
    }
    func_ov000_02357878(w, 0x80, 0x98, 0x1000);
    func_ov000_02358224(w, 0, 0x1000);
    data_02087d04[2].x++;
    func_ov000_023580bc(w, TRUE);
    w->unk0++;
    if (func_02050018()) {
        if (!func_0205000c()) {
            func_02051628(0);
            func_ov000_0235650c(w);
            func_ov000_023597b8(0);
        } else {
            w->unk338 = 1;
            func_ov000_023599b0(0);
        }
    }
}

#pragma thumb reset
