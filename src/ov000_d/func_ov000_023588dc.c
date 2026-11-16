#include <ov000_d/scene.h>

void Task_DestroyByTag(s32 a);
void func_ov000_02355dac(s32 mode, s32 sub);

#pragma thumb on

void func_ov000_023588dc(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *p;
    u32 step;
    s32 i;

    if (w->unk0 >= 0x28) {
        s32 mode = w->unkC;
        s32 sel = w->unk14;
        Task_DestroyByTag(0x1000);
        func_ov000_02355dac(mode, sel);
        return;
    }
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
    if (w->unkC != 2) {
        p = &w->pos[5];
        func_ov000_023578d0(w, p->x, p->y);
    }
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
    gDisplaySub[2].x++;
    func_ov000_023580bc(w, TRUE);
    w->unk0++;
}

#pragma thumb reset
