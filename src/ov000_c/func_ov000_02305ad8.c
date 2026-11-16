#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blkA;
    UnkStruct_Blk blkB;
    u8 pad36C[0x99C - 0x36C];
    UnkStruct_Spr sprA;
    UnkStruct_Spr sprB;
} UnkStruct_02305ad8;

extern void *data_ov000_02369ba0;
extern void *data_ov000_02369e9c;
extern u8 data_ov000_02366810[], data_ov000_02369e94[], data_ov000_02366828[], data_ov000_0236683c[];
extern u8 data_ov000_02366850[], data_ov000_02369ea4[];

void func_ov000_02305a78(void);
void func_ov000_02305914(void);
void func_ov000_02305678(void);
u16 func_0206d2d0(void *a, s32 b, s32 c);
void func_ov000_02305904(UnkStruct_02305ad8 *w);

UnkStruct_02305ad8 *func_ov000_02305ad8(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02305ad8 *w;
    vu16 zero;
    u16 v;

    task = func_020372d0(func_0206c3d0, func_ov000_02305a78, 0, 0xf7, 2, sizeof(UnkStruct_02305ad8));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02305ad8));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_02366810, 4, &data_ov000_02369e9c, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x36);
    func_02069918(w, 0x14);
    func_02069818(w, 2);
    w->base.spr0.unk2C |= 0x80000;
    func_02069a34(w, &w->blkA, data_ov000_02366828, 0, 0xb, 0xa, data_ov000_02369e94, data_ov000_02369ba0);
    func_02069a34(w, &w->blkB, data_ov000_0236683c, 0, 0xc, 9, data_ov000_02369e94, data_ov000_02369ba0);
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    func_02069d38(&w->sprA, &w->sprB, data_ov000_02366850, 0, &data_ov000_02369e9c, data_ov000_02369ba0);
    func_02033550(&w->sprA, 0);
    func_02033550(&w->sprB, 0);
    w->sprA.unk46 = 0x14;
    w->sprB.unk46 = 0x14;
    v = func_0206d2d0(data_ov000_02369e9c, 0, 0x36);
    w->sprA.unk44 = v;
    w->sprB.unk44 = v;
    w->sprA.unk2C |= 0x10;
    w->sprB.unk2C |= 0x10;
    func_02069e68(&w->sprA, &w->sprB, 1, data_ov000_02369ea4);
    w->base.unk70 = func_ov000_02305914;
    if (w->base.unk1E0->unk4 & 1)
        w->base.unk5C ^= 1;
    func_0206e3c0(&w->base.coll0, -0x78, 0, -0x70, 8);
    func_0206e2b4(&w->base.coll0, 0, -16);
    w->base.coll0.unkA |= 0x800;
    w->base.coll0.unk1C = func_ov000_02305678;
    w->base.coll0.unk18 = w;
    w->base.unk1E0->unk7 = 0x40;
    w->base.unk14 += (w->base.unk1E0->unk7 + w->base.unk1E0->unk8 + w->base.unk1E0->unk9) << 8;
    if (w->base.unk1E0->unk4 & 4)
        w->base.unk34 = 0xd00;
    w->base.unk58 |= 0x100;
    w->base.unk54 |= 0x10;
    w->base.unk1DC = func_0203663c();
    func_ov000_02305904(w);
    return w;
}
