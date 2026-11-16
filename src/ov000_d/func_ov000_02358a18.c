#include <ov000_d/scene.h>

typedef struct {
    u8 pad0[8];
    s16 unk8;
} UnkStruct_02358a18_Bright;

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
} UnkStruct_02358a18_Pad;

extern UnkStruct_02358a18_Bright data_02087d44;
extern UnkStruct_02358a18_Bright data_0208926c;
extern UnkStruct_02358a18_Pad gPad;

BOOL func_ov000_02356538(UnkStruct_Scene *w);
BOOL func_ov000_023564cc(UnkStruct_Scene *w);
BOOL func_ov000_02356590(UnkStruct_Scene *w);
void Task_DestroyByTag(s32 a);
void func_ov000_02355e14(s32 mode, s32 sub);
void Task_DestroyRange(s32 a, s32 b);
void func_ov000_02356694(s32 x, s32 y, s32 a, s32 b);
void SysSound_StopBGM(s32 a);
void func_020520e0(s32 a);
void func_ov000_023599b0(s32 a);
void func_ov000_023596f4(s32 a);

#pragma thumb on

void func_ov000_02358a18(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *p;
    u32 step;
    s32 i;
    s32 limit;
    BOOL back = FALSE;
    BOOL next = FALSE;

    if ((u32)(w->unkC - 1) <= 1) {
        if (w->unk4 >= 0xe2) {
            if (func_ov000_02356538(w)) {
                s32 mode = w->unkC;
                s32 sel = w->unk14;
                Task_DestroyByTag(0x1000);
                func_ov000_02355e14(mode, sel);
                return;
            }
            Task_DestroyRange(0x3000, 0x3000);
            data_02087d44.unk8 = 0;
            data_0208926c.unk8 = data_02087d44.unk8;
            back = TRUE;
        }
        if (w->unkC == 2 && func_ov000_023564cc(w)) {
            back = TRUE;
        }
        if (func_ov000_02356590(w)) {
            next = TRUE;
        }
    } else {
        if (w->unk14 == 2) {
            limit = 0x78;
        } else {
            limit = 0xe2;
        }
        if (limit - w->unk0 > 0x10 && w->unk0 >= 0x20 && (gPad.trg & 1)) {
            w->unk0 = limit - 0x10;
        }
        if (limit - w->unk0 == 0x10) {
            func_ov000_02356694(0, 0x10, 0x10, 0);
            if (w->unkC == 0) {
                SysSound_StopBGM(0x10);
            }
        }
        if (w->unk0 >= limit) {
            s32 mode = w->unkC;
            s32 sel = w->unk14;
            Task_DestroyByTag(0x1000);
            func_ov000_02355e14(mode, sel);
            return;
        }
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
    p = &w->pos[5];
    func_ov000_023578d0(w, p->x, p->y);
    func_ov000_02357970(w);
    if (w->unk10 == 2) {
        step = 0x8000;
    } else {
        step = 0x5555;
    }
    for (i = 1; i < w->unk10; i++) {
        func_ov000_02358384(w, (w->unk14 + i) % w->unk10, step * i);
    }
    func_ov000_02357844(w, 0x80, 0x98);
    func_ov000_02358224(w, 0, 0x1000);
    gDisplaySub[2].x++;
    func_ov000_023580bc(w, TRUE);
    if (w->unk0 == 8 && (u32)(w->unkC - 1) <= 1) {
        switch (w->unk14) {
        case 0:
            func_020520e0(6);
            break;
        case 1:
            func_020520e0(7);
            break;
        }
    }
    w->unk0++;
    if ((u32)(w->unkC - 1) <= 1) {
        if (func_ov000_02356538(w)) {
            if (w->unk8 > 0x10) {
                if (w->unk4 == 0) {
                    func_ov000_02356694(0, 0x10, 0x10, 0xd2);
                }
                w->unk4++;
            }
            w->unk8++;
        } else {
            w->unk8 = 0;
            w->unk4 = 0;
        }
    }
    if ((u32)(w->unkC - 1) <= 1 && w->unk4 == 0 && w->unk8 == 0 && (gPad.trg & 2)) {
        back = TRUE;
    }
    if (back) {
        func_ov000_023599b0(0);
    } else if (next) {
        func_ov000_023596f4(0);
    }
}

#pragma thumb reset
