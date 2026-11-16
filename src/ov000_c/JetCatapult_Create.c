#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_0230e8e4;

extern void *data_ov000_02369ba0;
extern void *data_ov000_0236a024;
extern u8 data_ov000_02366bbc[], data_ov000_0236a02c[], data_ov000_02366bdc[];

void JetCatapult_Destructor(void);
void JetCatapult_OnHit(void);
void ObjData_Load(void *a, void *b, void *c);
void JetCatapult_Action_Unk_ov000_0230e8b0(UnkStruct_0230e8e4 *w);
void JetCatapult_CreatePart_ov000_0230e354(UnkStruct_0230e8e4 *w, s32 x, s32 y);

UnkStruct_0230e8e4 *JetCatapult_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230e8e4 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, JetCatapult_Destructor, 0, 0xf6, 2, sizeof(UnkStruct_0230e8e4));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230e8e4));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366bbc, 0xffff, data_ov000_0236a02c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x32);
    StageObj_SetOAMOrder(w, 0x14);
    w->base.spr0.unk2C |= 0x200;
    w->base.spr1.unk2C |= 0x200;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = JetCatapult_OnHit;
    w->base.coll0.unkA |= 0x80;
    func_02068e40(w, -20, -5, 0x1c, 9);
    ObjData_Load(&data_ov000_0236a024, data_ov000_02366bdc, data_ov000_02369ba0);
    w->base.unk1C8 = data_ov000_0236a024;
    w->base.unk1B8 = w;
    w->base.unk1D8 = 10;
    w->base.unk1DA = 3;
    w->base.unk1D4 = -0x2a;
    w->base.unk1D6 = -0xc;
    w->base.unk1C4 |= 1;
    w->base.unk5C ^= 1;
    w->base.unk58 |= 0x4080;
    JetCatapult_Action_Unk_ov000_0230e8b0(w);
    w->base.unk1DC = Sound_AllocHandle();
    JetCatapult_CreatePart_ov000_0230e354(w, w->base.unk10 + (w->base.unk1E0->unk6 << 8), w->base.unk14 + (w->base.unk1E0->unk7 << 8));
    return w;
}
