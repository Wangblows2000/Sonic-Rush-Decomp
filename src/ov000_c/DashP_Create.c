#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_02307adc;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366900[], data_ov000_02369f34[];

void MapObject_Destructor(void);
void DashP_OnHit(void);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void DashP_Action_Unk_ov000_02307aa4(UnkStruct_02307adc *w);

UnkStruct_02307adc *DashP_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02307adc *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_02307adc));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02307adc));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366900, 8, data_ov000_02369f34, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 2);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = DashP_OnHit;
    w->base.unk58 |= 0x2100;
    switch (w->base.unk1E0->unk2) {
    case 0x5c:
    default:
        if (w->base.unk1E0->unk4 & 1)
            w->base.unk5C |= 1;
        else
            w->base.unk5C &= ~1;
        break;
    case 0x60:
        w->base.unk5C |= 2;
        if (w->base.unk1E0->unk4 & 1)
            w->base.unk5C |= 1;
        else
            w->base.unk5C &= ~1;
        break;
    case 0x61:
        w->base.unk5C &= ~1;
        if (w->base.unk1E0->unk4 & 1)
            w->base.unk5C |= 2;
        break;
    case 0x62:
        w->base.unk5C |= 1;
        if (w->base.unk1E0->unk4 & 1)
            w->base.unk5C |= 2;
        break;
    case 0x98:
    case 0x9a:
        w->base.unk5C &= ~1;
        break;
    case 0x99:
    case 0x9b:
        w->base.unk5C |= 1;
        break;
    }
    switch (w->base.unk1E0->unk2) {
    case 0x5d:
    case 0x5e:
    case 0x5f:
    case 0x60:
    default:
        w->base.unk8 = 0;
        break;
    case 0x61:
    case 0x62:
        w->base.unk8 = 1;
        break;
    case 0x5c:
        w->base.unk8 = 2;
        if (Stage_IsGravityFlipped(w->base.unk10, w->base.unk14))
            w->base.unk5C ^= 2;
        break;
    case 0x98:
    case 0x99:
        w->base.unk8 = 4;
        break;
    case 0x9a:
    case 0x9b:
        w->base.unk8 = 3;
        break;
    }
    DashP_Action_Unk_ov000_02307aa4(w);
    return w;
}
