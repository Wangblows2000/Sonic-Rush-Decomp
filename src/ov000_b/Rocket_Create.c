#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366e14[];
extern u8 data_ov000_0236a1e4[];

void MapObject_Destructor(void);
void Rocket_OnHit(void);
void Rocket_OnHit_Box1(void);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void Rocket_Action_Unk_ov000_02313ec8(UnkStruct_StageObj *w);

UnkStruct_StageObj *Rocket_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0xf6, 2, sizeof(UnkStruct_StageObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366e14, 0xffff, data_ov000_0236a1e4, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0xb);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    func_02068e40(w, -0xc, -0xc, 0xc, 0xc);
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unk1C = Rocket_OnHit;
    w->coll0.unkA |= 0x800;
    ObjRect_SetAtkDefFlags(&w->coll1, 0, -0x10);
    w->coll1.unk1C = Rocket_OnHit_Box1;
    w->coll1.unkA |= 0x800;
    w->coll1.unk28 = 0x21;
    if (w->unk1E0->unk4 & 1)
        w->unk5C ^= 1;
    w->unk58 |= 0x2110;
    w->unk58 |= 0x80000;
    w->unk58 &= ~0x80;
    w->unk5C |= 0x100;
    if (Stage_IsGravityFlipped(w->unk10, w->unk14))
        w->unk5 = 0x80;
    w->unk1DC = Sound_AllocHandle();
    Rocket_Action_Unk_ov000_02313ec8(w);
    return w;
}
