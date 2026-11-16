#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
} UnkStruct_0236aad8;

extern void *gActComArchive;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_0236aad8 gCamera;
extern u8 data_ov000_023672bc[];
extern u8 data_ov000_0236a6f8[];

void func_ov000_0231fdec(void);

UnkStruct_EffObj *func_ov000_0231fe0c(void) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (gGameState.unk18 & 0x20)
        return NULL;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_023672bc, 0xffff, data_ov000_0236a6f8, gActComArchive);
    StageObj_AllocSpritePalette(w, 0, 0xc);
    StageObj_SetAnimation(w, (u16)(Rand() & 3));
    w->unk10 = gCamera.unk4 - 0x1800;
    w->unk14 = gCamera.unk8 + ((Rand() & 0xff) << 8);
    w->unk2E = (Rand() & 0x3ff) + 0x700;
    w->unk30 = (Rand() & 0x3ff) - 0x200;
    StageObj_SetOAMPriority(w, 1);
    StageObj_SetOAMOrder(w, 6);
    w->unk5C |= 0x20;
    w->unk5C |= 1;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    w->unk68 = func_ov000_0231fdec;
    return w;
}
