#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Spr s0;
    UnkStruct_Spr s1;
    UnkStruct_Spr s2;
    UnkStruct_Spr s3;
} UnkStruct_02317ac8;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366fd8[];
extern u8 data_ov000_0236a264[];
extern u8 data_ov000_02366ff0[];
extern u8 data_ov000_02367008[];

void func_ov000_02317a40(void);
void func_ov000_0231798c(void);
void func_ov000_02317764(void);
BOOL func_ov000_02345758(s32 x, s32 y);
void func_ov000_0231797c(UnkStruct_02317ac8 *w);

UnkStruct_02317ac8 *func_ov000_02317ac8(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02317ac8 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_02317a40, 0, 0x500, 2, sizeof(UnkStruct_02317ac8));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02317ac8));
    func_ov000_022fba50(w, a, b, c);
    func_02069f1c(w, data_ov000_02366fd8, 2, data_ov000_0236a264, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x2c);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    w->base.unk70 = func_ov000_0231798c;

    func_02069d38(&w->s0, &w->s1, data_ov000_02366ff0, 0xb, data_ov000_0236a264, data_ov000_02369ba0);
    func_02033550(&w->s0, 1);
    func_02033550(&w->s1, 1);
    w->s0.unk46 = 9;
    w->s1.unk46 = 9;
    w->s0.unk2C &= ~0x60000;
    w->s1.unk2C &= ~0x60000;
    w->s0.unk2C |= 0x20000;
    w->s1.unk2C |= 0x20000;
    w->s0.unk44 = w->base.spr0.unk44;
    w->s1.unk44 = w->base.spr0.unk44;
    w->s0.unk2C |= 0x200;
    w->s1.unk2C |= 0x200;

    func_02069d38(&w->s2, &w->s3, data_ov000_02367008, 0xa, data_ov000_0236a264, data_ov000_02369ba0);
    func_02033550(&w->s2, 2);
    func_02033550(&w->s3, 2);
    w->s2.unk46 = 0x15;
    w->s3.unk46 = 0x15;
    w->s2.unk44 = w->base.spr0.unk44;
    w->s3.unk44 = w->base.spr0.unk44;
    w->s2.unk2C |= 0x200;
    w->s3.unk2C |= 0x200;

    func_0206e3c0(&w->base.coll0, -4, 0x30, 4, 0x38);
    func_0206e2b4(&w->base.coll0, 0, -0x10);
    w->base.coll0.unk1C = func_ov000_02317764;
    w->base.coll0.unkA |= 0x800;
    w->base.coll0.unk18 = w;
    func_0206e3c0(&w->base.coll1, -4, -0x38, 4, -0x30);
    func_0206e2b4(&w->base.coll1, 0, -0x10);
    w->base.coll1.unk1C = func_ov000_02317764;
    w->base.coll1.unkA |= 0x800;
    w->base.coll1.unk18 = w;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x100;
    if (func_ov000_02345758(w->base.unk10, w->base.unk14))
        w->base.unk5 = 0x80;
    func_ov000_0231797c(w);
    return w;
}
