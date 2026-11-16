#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0xD6 - 0xC];
    s16 unkD6;
    u8 padD8[0x104 - 0xD8];
} UnkStruct_0231979c_Sub;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_0231979c_Sub unk134;
} UnkStruct_0231979c;

extern void *data_ov000_02369ba0;
extern UnkStruct_StageObj *data_ov000_0236a2fc;
extern u8 data_ov000_0236a300[], data_ov000_02367110[], data_ov000_0236a310[];

void EffShadow_State_Main(void);
void StageObj_Load3dBAC(void *w, UnkStruct_0231979c_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void StageObj_AcquireSharedTexture(UnkStruct_0231979c_Sub *s, void *a, s32 b, s32 c);

void EffShadow_Create(UnkStruct_StageObj *arg, void *n) {
    UnkStruct_Task *task;
    UnkStruct_0231979c *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_0231979c));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231979c));
    StageObj_Load3dBAC(w, &w->unk134, data_ov000_02367110, 0, 0, data_ov000_0236a300, data_ov000_02369ba0);
    StageObj_AcquireSharedTexture(&w->unk134, data_ov000_0236a310, 0x80, 0x10);
    w->unk134.unkA = 4;
    w->unk134.unkB = 0;
    w->base.unk5C |= 0x200;
    w->unk134.unkD6 = 0x16;
    w->base.unk88 = arg;
    w->base.unk10 = arg->unk10;
    w->base.unk18 = arg->unk18 - 0x800;
    w->base.unk80 = n;
    w->base.unk80 = data_ov000_0236a2fc;
    if (w->base.unk80 != NULL) {
        w->base.unk14 = ((UnkStruct_StageObj *)w->base.unk80)->unk14 - 0x1200;
        w->base.unk1 = ((UnkStruct_StageObj *)w->base.unk80)->unk1;
    }
    w->base.unk58 |= 0x100;
    w->base.unk54 |= 2;
    w->base.unk54 |= 0x10;
    w->base.unk5C |= 0x100;
    w->base.unk68 = EffShadow_State_Main;
}
