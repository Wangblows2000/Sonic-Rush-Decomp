#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_Spr s0;
    UnkStruct_Spr s1;
    UnkStruct_Spr s2;
    UnkStruct_Spr s3;
    u8 pad254[0x268 - 0x254];
    s32 unk268;
    s32 unk26C;
    u32 unk270;
    u8 pad274[4];
} UnkStruct_0232d620;

typedef struct {
    u8 pad0[0x20C];
    u32 unk20C;
    u8 pad210[2];
    s8 unk212;
    s8 unk213;
} UnkStruct_0232d620_Ply;

extern void *data_022c4488;
extern UnkStruct_Task *data_ov000_0236a988;
extern u8 data_ov000_02368058[], data_ov000_0236a990[], data_ov000_02368078[], data_ov000_02368098[];

void func_020371c8(UnkStruct_Task *t);
void func_ov000_0232d5e4(void);
void func_ov000_0232d4c8(void);
UnkStruct_0232d620_Ply *func_ov000_02342358(s32 a);
void func_ov000_02336f28(u32 *out, s32 a, s32 b);
void func_ov000_0232d2dc(UnkStruct_0232d620 *w);

void func_ov000_0232d620(void) {
    UnkStruct_Task *task;
    UnkStruct_0232d620 *w;
    UnkStruct_0232d620_Ply *p;
    vu16 zero;

    if (data_ov000_0236a988 != NULL)
        func_020371c8(data_ov000_0236a988);
    task = func_020372d0(func_0206c3d0, func_ov000_0232d5e4, 0, 0xa00, 4, sizeof(UnkStruct_0232d620));
    if (task == func_02006abc(0))
        return;
    data_ov000_0236a988 = task;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0232d620));
    func_02069f1c(w, data_ov000_02368058, 0x20, data_ov000_0236a990, data_022c4488);
    func_0206a054(w, 0, 0xa);
    func_02069918(w, 3);
    func_020698f0(w, 0);
    func_02069d38(&w->s0, &w->s1, data_ov000_02368078, 0, data_ov000_0236a990, data_022c4488);
    func_0206990c(&w->s0, &w->s1, 3);
    func_020698bc(&w->s0, &w->s1, 0);
    w->s0.unk44 = w->base.spr0.unk44;
    w->s1.unk44 = w->base.spr0.unk44;
    w->s0.unk34 = w->base.spr0.unk34 + 0x280;
    w->s1.unk34 = w->base.spr1.unk34 + 0x280;
    w->s0.unk2C |= 0x200;
    w->s1.unk2C |= 0x200;
    func_02069d38(&w->s2, &w->s3, data_ov000_02368098, 0, data_ov000_0236a990, data_022c4488);
    func_0206990c(&w->s2, &w->s3, 3);
    func_020698bc(&w->s2, &w->s3, 0);
    w->s2.unk44 = w->base.spr0.unk44;
    w->s3.unk44 = w->base.spr0.unk44;
    w->s2.unk34 = w->base.spr0.unk34 + 0x380;
    w->s3.unk34 = w->base.spr1.unk34 + 0x380;
    w->s2.unk2C |= 0x200;
    w->s3.unk2C |= 0x200;
    w->base.unk70 = func_ov000_0232d4c8;
    w->base.unk10 = 0x8000;
    w->base.unk14 = 0x2800;
    w->base.unk5C |= 1;
    w->base.unk5C |= 0x80;
    w->base.unk58 |= 0x100;
    w->base.unk54 |= 0x12;
    p = func_ov000_02342358(1);
    if (p != NULL) {
        w->unk26C = w->unk268 = p->unk212 - p->unk213;
        if (p->unk20C & 1)
            w->base.unk54 |= 0x4000;
    } else {
        w->unk26C = w->unk268 = 0;
    }
    func_ov000_02336f28(&w->unk270, w->unk268, 100);
    func_02033550(&w->s0, (u16)(((w->unk270 & 0xf0) >> 4) + 1));
    func_02033550(&w->s1, (u16)(((w->unk270 & 0xf0) >> 4) + 1));
    func_02033550(&w->s2, (u16)((w->unk270 & 0xf) + 1));
    func_02033550(&w->s3, (u16)((w->unk270 & 0xf) + 1));
    func_ov000_0232d2dc(w);
}
