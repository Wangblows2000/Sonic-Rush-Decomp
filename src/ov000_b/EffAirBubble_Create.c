#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x30 - 0x18];
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x134 - 0x6C];
} UnkStruct_02329f50;

extern u32 gRandSeed;
extern void *gZoneActArchive;
extern u8 data_ov000_0236aa08[];
extern u8 data_ov000_02367f60[];

void EffAirBubble_State_Rise(void);


void EffAirBubble_Create(UnkStruct_StageObj *arg, s16 x, s16 y, s32 type, s32 t) {
    UnkStruct_Task *task;
    UnkStruct_02329f50 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02329f50));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02329f50));
    StageObj_Load2dBAC(w, data_ov000_02367f60, 1, data_ov000_0236aa08, gZoneActArchive);
    StageObj_AllocSpritePalette(w, 0, 0x17);
    StageObj_SetAnimation(w, (u16)(type + 2));
    StageObj_SetOAMOrder(w, 9);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10 + (x << 8) + ((3 - (Rand() & 6)) << 8);
    w->unk14 = arg->unk14 + (y << 8);
    w->unk30 = 0xff - (Rand() & 0xfe);
    w->unkC = t + 6;
    w->unk8 = 0x2bf - (Rand() & 0xfe);
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    w->unk68 = EffAirBubble_State_Rise;
}
