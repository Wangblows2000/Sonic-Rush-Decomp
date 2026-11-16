#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blk[11];
    u8 pad99C[0xA2C - 0x99C];
} UnkStruct_0230614c;

typedef struct {
    u8 b[11];
} UnkStruct_0230614c_Tbl;

extern void *data_ov000_02369ba0;
extern UnkStruct_0230614c_Tbl data_ov000_023667d8;
extern u8 data_ov000_02366868[], data_ov000_02369e9c[], data_ov000_02369e94[], data_ov000_02366880[];

void func_ov000_022fb9c8(void);
void func_ov000_0230613c(UnkStruct_0230614c *w);
UnkStruct_StageObj *func_ov000_022fad3c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);

UnkStruct_0230614c *func_ov000_0230614c(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_0230614c_Tbl tbl = data_ov000_023667d8;
    UnkStruct_Task *task;
    UnkStruct_0230614c *w;
    UnkStruct_StageObj *o;
    vu16 zero;
    u16 i;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0xf6, 2, sizeof(UnkStruct_0230614c));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230614c));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_02366868, 0x36, data_ov000_02369e9c, data_ov000_02369ba0);
    func_0206a054(w, 1, 0x4c);
    func_02069918(w, 0x14);
    func_02069818(w, 1);
    w->base.spr0.unk2C |= 0x80000;
    for (i = 0; i < 11; i++) {
        if (i == 2)
            continue;
        func_02069a34(w, &w->blk[i], data_ov000_02366880, 0, i, tbl.b[i], data_ov000_02369e94, data_ov000_02369ba0);
    }
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    w->base.unk58 |= 0x2100;
    if (w->base.unk1E0->unk4 & 1) {
        w->base.unkC = 0x8000;
        w->base.unk54 ^= 1;
    } else {
        w->base.unkC = 0;
    }
    w->base.unk8 = w->base.unkC;
    func_ov000_0230613c(w);
    o = func_ov000_022fad3c(0xed, w->base.unk10, w->base.unk14, w->base.unk1E0->unk4, 0, w->base.unk1E0->unk7,
                            w->base.unk1E0->unk8, w->base.unk1E0->unk9, 0);
    if (o != NULL) {
        o->unk88 = w;
        o->unk8 = w->base.unk8;
    }
    o = func_ov000_022fad3c(0xee, w->base.unk10, w->base.unk14, 0, w->base.unk1E0->unk6, 0, 0, 0, 0);
    if (o != NULL)
        o->unk88 = w;
    w->base.unk1DC = func_0203663c();
    return w;
}
