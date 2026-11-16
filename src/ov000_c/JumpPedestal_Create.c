#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_0230ed60;

extern void *data_ov000_02369ba0;
extern void *data_ov000_0236a04c;
extern void *data_ov000_0236a044;
extern void *data_ov000_0236a03c;
extern u8 data_ov000_02366bf0[], data_ov000_0236a054[], data_ov000_02366c10[], data_ov000_02366c28[];
extern u8 data_ov000_02366c40[];

void JumpPedestal_Destructor(void);
void ObjData_Load(void *a, void *b, void *c);
void JumpPedestal_Action_Unk_ov000_0230ecc8(UnkStruct_0230ed60 *w);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);

UnkStruct_0230ed60 *JumpPedestal_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230ed60 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, JumpPedestal_Destructor, 0, 0x500, 2, sizeof(UnkStruct_0230ed60));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230ed60));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366bf0, 0xa, data_ov000_0236a054, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 2);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    w->base.unk58 |= 0x2100;
    w->base.unk54 |= 2;
    if (!(w->base.unk1E0->unk4 & 4))
        w->base.unk5C &= ~1;
    switch ((u8)(w->base.unk1E0->unk4 & 3)) {
    default:
        ObjData_Load(&data_ov000_0236a04c, data_ov000_02366c10, data_ov000_02369ba0);
        w->base.unk1C8 = data_ov000_0236a04c;
        break;
    case 1:
        ObjData_Load(&data_ov000_0236a044, data_ov000_02366c28, data_ov000_02369ba0);
        w->base.unk1C8 = data_ov000_0236a044;
        break;
    case 2:
        ObjData_Load(&data_ov000_0236a03c, data_ov000_02366c40, data_ov000_02369ba0);
        w->base.unk1C8 = data_ov000_0236a03c;
        break;
    }
    w->base.unk1B8 = w;
    w->base.unk1D8 = 6;
    w->base.unk1DA = 4;
    w->base.unk1D4 = -0x18;
    w->base.unk1D6 = -0x14;
    JumpPedestal_Action_Unk_ov000_0230ecc8(w);
    if (Stage_IsGravityFlipped(w->base.unk10, w->base.unk14)) {
        w->base.unk5C |= 2;
        w->base.unk5 = 0x80;
    }
    return w;
}
