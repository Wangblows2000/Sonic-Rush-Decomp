#include <ov000_d/scene.h>

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
} UnkStruct_023593bc_Pad;

extern UnkStruct_023593bc_Pad data_022503e0;

void func_ov000_023599b0(s32 a);

#pragma thumb on

void func_ov000_023593bc(void) {
    UnkStruct_Scene *w = data_022b4574->work;
    UnkStruct_ScenePos *p;
    s32 ang;
    s32 x;
    BOOL done;

    if (w->unkC != 1 && w->unkC != 2 && (data_022503e0.trg & 1)) {
        w->unk340 = 1;
    }
    if (w->unk340) {
        w->unk0 = 0x10;
        func_02040e48(&w->pos[0]);
        func_02040e48(&w->pos[1]);
        func_02040e48(&w->pos[2]);
        func_02040e48(&w->pos[3]);
        func_02040e48(&w->pos[4]);
        func_02040e48(&w->pos[5]);
    }
    ang = (w->unk0 << 14) >> 4;
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
    done = FALSE;
    if (w->unk0 >= 6) {
        func_02040d14(&w->pos[6]);
        func_ov000_02357ec8(w, w->unk14, w->pos[6].unk4);
        func_02040d14(&w->pos[7]);
        x = w->pos[7].x;
        if (!w->unk20) {
            x = -x;
        }
        func_ov000_023582e8(w, w->unk14, x);
        func_02040d14(&w->pos[0]);
        func_02040d14(&w->pos[1]);
        func_02040d14(&w->pos[2]);
        func_02040d14(&w->pos[3]);
        func_02040d14(&w->pos[4]);
        func_02040d14(&w->pos[5]);
        done = TRUE;
        p = &w->pos[0];
        func_ov000_023579fc(w, p->x, p->y);
        if (!func_02040cf4(p)) {
            done = FALSE;
        }
        p = &w->pos[1];
        func_ov000_023579c8(w, p->x, p->y);
        if (!func_02040cf4(p)) {
            done = FALSE;
        }
        p = &w->pos[2];
        func_ov000_0235793c(w, p->x, p->y);
        if (!func_02040cf4(p)) {
            done = FALSE;
        }
        p = &w->pos[3];
        func_ov000_02357908(w, p->x, p->y);
        if (!func_02040cf4(p)) {
            done = FALSE;
        }
        p = &w->pos[4];
        func_ov000_02357994(w, p->x, p->y);
        if (!func_02040cf4(p)) {
            done = FALSE;
        }
        p = &w->pos[5];
        func_ov000_023578d0(w, p->x, p->y);
        if (!func_02040cf4(p)) {
            done = FALSE;
        }
    }
    func_ov000_02358224(w, 0, 0x1000);
    w->unk330 += 0x20;
    if (w->unk330 > 0) {
        w->unk330 = 0;
    }
    func_ov000_02358108(w, w->unk330);
    w->unk0++;
    if (done && w->unk0 >= 0x10) {
        func_ov000_02356e7c(&w->mdl, 0);
        func_ov000_02357ec8(w, w->unk14, 0x1000);
        func_ov000_023582e8(w, w->unk14, 0);
        w->unk340 = 0;
        func_ov000_023599b0(0);
    }
}

#pragma thumb reset
