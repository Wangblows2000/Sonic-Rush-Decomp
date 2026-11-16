#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    s8 unk210;
    s8 unk211;
    s8 unk212;
    u8 pad213;
    u8 unk214;
    u8 pad215;
    u16 unk216;
    u16 unk218;
    u16 unk21A;
    u16 unk21C;
    u8 pad21E[0x234 - 0x21E];
} UnkStruct_023099e8;

typedef struct {
    u8 pad0[0x3A];
    s8 unk3A;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 data_ov000_0236aad8;

void func_ov000_023099c0(void);
u8 func_ov000_02307e84(UnkStruct_023099e8 *w);
void func_ov000_023423a4(s32 a, UnkStruct_023099e8 *w, u8 b);
void func_ov000_023099b0(UnkStruct_023099e8 *w);

UnkStruct_023099e8 *func_ov000_023099e8(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_023099e8 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_023099c0, 0, 0x4f6, 2, sizeof(UnkStruct_023099e8));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023099e8));
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    func_0206e2b4(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = NULL;
    w->base.coll0.unkA |= 0x800;
    w->base.unk4A = 0x300;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    w->unk216 = 0xffff;
    w->unk218 = 0xffff;
    w->unk21A = 0xffff;
    w->unk21C = 0xffff;
    w->unk212 = w->base.unk1E0->unk6;
    if (w->base.unk1E0->unk4 & 1)
        w->unk20C |= 1;
    w->unk210 = data_ov000_0236aad8.unk3A;
    w->unk211 = -1;
    w->unk214 = func_ov000_02307e84(w);
    func_ov000_023423a4(1, w, w->unk214);
    func_ov000_023099b0(w);
    return w;
}
