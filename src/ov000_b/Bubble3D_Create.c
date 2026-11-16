#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0x104 - 0xC];
} UnkStruct_02329d38_Sub;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x30 - 0x1C];
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x134 - 0x6C];
    UnkStruct_02329d38_Sub unk134;
} UnkStruct_02329d38;

extern u32 gRandSeed;
extern void *gZoneActArchive;
extern u8 data_ov000_0236aa08[];
extern u8 data_ov000_02367f44[];
extern u16 data_ov000_023645c0[];

void func_ov000_02329be0(void);
void StageObj_Load3dBAC(void *w, UnkStruct_02329d38_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void func_ov000_022c5e80(s32 a, s32 b, s32 c, s32 *d, s32 *e, s16 *f);


UnkStruct_02329d38 *Bubble3D_Create(UnkStruct_StageObj *arg, s16 x, s16 y, s32 type, s32 unused, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_02329d38 *w;
    s32 o10;
    s32 o14;
    s16 o0C;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02329d38));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02329d38));
    StageObj_Load3dBAC(w, &w->unk134, data_ov000_02367f44, data_ov000_023645c0[type], 0x10, data_ov000_0236aa08, gZoneActArchive);
    StageObj_SetAnimation(w, (u16)(type + 2));
    w->unk134.unkA = 0x14;
    w->unk134.unkB = 7;
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10 + (x << 8) + ((3 - (Rand() & 6)) << 8);
    w->unk14 = arg->unk14 + (y << 8);
    w->unk30 = 0xff - (Rand() & 0xfe);
    w->unk8 = 0x2bf - (Rand() & 0xfe);
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    w->unk68 = func_ov000_02329be0;
    func_ov000_022c5e80(a, b, w->unk10 << 4, &o10, &o14, &o0C);
    w->unk10 = o10 >> 4;
    w->unk18 = o14 >> 4;
    return w;
}
