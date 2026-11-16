#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
    s32 unk20;
    u8 pad24[4];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[2];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x88 - 0x6C];
    void *unk88;
    u8 pad8C[0x94 - 0x8C];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
} UnkStruct_02332b3c;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 pad1C[4];
    u32 unk20;
} UnkStruct_022c4560;

extern u32 gRandSeed;
extern void *gActComArchive;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_023684f0[];
extern u8 data_ov000_0236a9f8[];

void StageObj_SetAnimation(void *w, s32 a);
void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void func_ov000_023329ec(void);


UnkStruct_02332b3c *func_ov000_02332b3c(UnkStruct_StageObj *arg, s16 x, s16 y, s32 unused) {
    UnkStruct_Task *task;
    UnkStruct_02332b3c *w;
    vu16 zero;
    u8 n;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02332b3c));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02332b3c));
    StageObj_Load2dBAC(w, data_ov000_023684f0, 6, data_ov000_0236a9f8, gActComArchive);
    StageObj_AllocSpritePalette(w, 2, 0x6e);
    StageObj_SetAnimation(w, 2);
    if (data_022c4560.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    StageObj_SetOAMOrder(w, 9);
    w->spr0.unk2C |= 0x400;
    w->spr1.unk2C |= 0x400;
    w->spr0.unk2A >>= 1;
    w->spr1.unk2A >>= 1;
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    if (arg->unk5 != 0)
        w->unk5C |= 2;
    w->unk88 = arg;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk1C = 0;
    w->unk20 = 0;
    w->unk2E = 0;
    w->unk30 = -0x80;
    n = arg->unk1 + arg->unk5;
    if (w->unk5C & 1)
        x = -x;
    Math_RotateVec2S16(&x, &y, n);
    x += 3 - (Rand() & 6);
    y += 3 - (Rand() & 6);
    w->unkC = (data_022c4560.unk20 & 0xf) << 12;
    w->unk28 = x;
    w->unk2A = y;
    w->unk8 = 0x200;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk68 = func_ov000_023329ec;
    return w;
}
