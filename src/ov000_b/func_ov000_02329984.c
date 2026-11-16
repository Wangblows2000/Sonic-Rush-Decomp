#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x34];
    s16 unk34;
    u8 pad36[0x5C - 0x36];
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
} UnkStruct_02329984_Arg;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
    u8 pad20[0x30 - 0x20];
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x88 - 0x6C];
    UnkStruct_02329984_Arg *unk88;
    u8 pad8C[0x134 - 0x8C];
} UnkStruct_02329984;

extern u32 gRandSeed;
extern void *gZoneActArchive;
extern u8 data_ov000_02367f2c[];
extern u8 data_ov000_0236aa08[];

void func_ov000_02329888(void);


void func_ov000_02329984(UnkStruct_02329984_Arg *arg, s32 x, s32 y, s32 dx, s32 unused, s32 t) {
    UnkStruct_Task *task;
    UnkStruct_02329984 *w;
    vu16 zero;
    UnkStruct_02329984_Arg *p;
    u16 r = Rand() & 1;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02329984));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02329984));
    StageObj_Load2dBAC(w, data_ov000_02367f2c, 1, data_ov000_0236aa08, gZoneActArchive);
    StageObj_AllocSpritePalette(w, 0, 0x17);
    StageObj_SetAnimation(w, (u16)(r + 2));
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 1);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = x + (dx << 8);
    w->unk14 = y + (0xfff - (Rand() & 0x1ffe));
    w->unk30 = 0x22f - (Rand() & 0x1fe);
    w->unk8 = t + 6;
    w->unk88 = arg;
    w->unkC = Rand() & 0x1ffff;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk1C = (x - 0x5400) + (0xfff - (Rand() & 0x1ffe));
    w->unk68 = func_ov000_02329888;
    p = w->unk88;
    w->unk30 = p->unk34 << 2;
    if (!(p->unk32C & 1))
        w->unk30 = -w->unk30;
}
