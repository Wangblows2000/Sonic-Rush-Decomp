#include <ov000_b/stageobj.h>
#include <ov000_b/res.h>

typedef struct {
    u8 pad0[0x10];
} UnkStruct_0236a790;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_Res *data_ov000_0236a6f0[];
extern UnkStruct_0236a790 data_ov000_0236a790[];
extern u8 data_ov000_02367324[], data_ov000_0236a718[];

void func_ov000_023208b0(void);

UnkStruct_EffObj *DecCloudW_Create(u32 a) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;
    u32 i;

    if (gGameState.unk18 & 0x20)
        return NULL;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    w->unk10 = (a & 7) << 14;
    StageObj_Load2dBAC(w, data_ov000_02367324, 0, data_ov000_0236a718, data_ov000_02369ba0);
    i = a & 3;
    StageObj_SetSharedSpriteVram(w, Res_GetElemVal(data_ov000_0236a6f0[10], i), &data_ov000_0236a790[i + 0x14]);
    StageObj_AllocSpritePalette(w, 0, 0x64);
    StageObj_SetAnimation(w, (u16)i);
    if (a > 3) {
        w->spr0.unk2C |= 8;
        w->spr1.unk2C |= 8;
    }
    if (a & 8) {
        w->unk14 = 0xc400;
        w->unk2E = -0x40;
        StageObj_SetOAMPriority(w, 3);
        StageObj_SetOAMOrder(w, 0x18);
    } else {
        w->unk14 = 0xe400;
        w->unk2E = -0x80;
        StageObj_SetOAMPriority(w, 0);
        StageObj_SetOAMOrder(w, 5);
    }
    w->unk5C |= 0x20;
    w->unk5C |= 1;
    w->unk5C |= 0x80;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x12;
    w->unk68 = func_ov000_023208b0;
    return w;
}
