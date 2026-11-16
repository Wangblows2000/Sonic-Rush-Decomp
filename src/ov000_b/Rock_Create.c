#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0x104 - 0xC];
} UnkStruct_023142f4_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_023142f4_Sub unk20C;
} UnkStruct_023142f4;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_022c4680;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
} UnkStruct_022c4534;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4680 gSaveData;
extern UnkStruct_022c4534 data_022c4534;
extern u8 data_ov000_02366e2c[], data_ov000_0236a1f4[], data_ov000_0236a1ec[], data_ov000_02366e44[], data_ov000_0236a1fc[];

void Rock_Destructor(void);
void StageObj_Load3dBAC(void *w, UnkStruct_023142f4_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void func_02068358(UnkStruct_023142f4_Sub *s, void *a, s32 b, s32 c);
void Rock_Action_Wait(UnkStruct_023142f4 *w);

UnkStruct_023142f4 *Rock_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_023142f4 *w;
    vu16 zero;

    if (p->unk4 & 0x80) {
        if (gSaveData.unk8 & 1)
            return NULL;
    }
    if ((b >> 8) < data_022c4534.unk1C + 0xc4)
        return NULL;
    task = Task_Create(StageObj_Main, Rock_Destructor, 0, 0x500, 2, sizeof(UnkStruct_023142f4));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023142f4));
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk4E = -0x100;
    StageObj_Load2dBAC(w, data_ov000_02366e2c, 0xffff, data_ov000_0236a1f4, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x2e);
    w->base.spr0.unk2C |= 0x80000;
    StageObj_Load3dBAC(w, &w->unk20C, data_ov000_02366e44, 0, 0, data_ov000_0236a1ec, data_ov000_02369ba0);
    func_02068358(&w->unk20C, data_ov000_0236a1fc, 0x200, 0x10);
    w->unk20C.unkA = 4;
    w->unk20C.unkB = 0;
    func_02068e40(w, -8, 0, 8, 0xc);
    if (w->base.unk1E0->unk7 < 0x28)
        w->base.unk18 = -(w->base.unk1E0->unk7 << 8);
    w->base.unk58 |= 0x2100;
    w->base.unk58 &= ~0x200;
    w->base.unk1DC = Sound_AllocHandle();
    Rock_Action_Wait(w);
    return w;
}
