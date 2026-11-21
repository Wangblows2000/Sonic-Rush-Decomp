#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_02316170_Tgt;

typedef struct {
    u8 pad0[5];
    u8 unk5;
    u8 pad6[0x10 - 6];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x3E8 - 0x18];
    UnkStruct_02316170_Tgt *unk3E8;
} UnkStruct_02316170_Arg;

extern void *gActComArchive;
extern u8 data_ov000_02366f30[], data_ov000_0236a24c[];

void EffSpecial_Destructor(void);
void EffSpecial_State_Main(void);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);

void EffSpecial_Create(UnkStruct_02316170_Arg *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, EffSpecial_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02366f30, 0xffff, data_ov000_0236a24c, gActComArchive);
    StageObj_AllocSpritePalette(w, 0, 0x4a);
    StageObj_SetOAMOrder(w, 0xa);
    if (Stage_IsGravityFlipped(arg->unk10, arg->unk14))
        w->unk5C |= 2;
    w->unk88 = arg;
    if (arg->unk3E8 != NULL) {
        w->unk10 = arg->unk3E8->unk10;
        w->unk14 = arg->unk3E8->unk14;
    }
    w->unkC = 0;
    w->unk5C |= 1;
    if (arg->unk5 != 0)
        w->unk14 += 0x4800;
    else
        w->unk14 -= 0x4800;
    w->unk5C |= 4;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    w->unk5C |= 0x20;
    w->unk44 = 8;
    w->unk46 = 0x180;
    w->unk68 = EffSpecial_State_Main;
}
