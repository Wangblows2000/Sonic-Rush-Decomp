#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x94 - 0x5E];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
    UnkStruct_Coll coll0;
    u8 pad160[0x19C - 0x160];
} UnkStruct_02321da4;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367354[], data_ov000_0236a708[];

void Decoration_Destructor(void);
void DecBird_OnHit(void);
void Decoration_InitFromMap(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);

UnkStruct_02321da4 *DecBird_Create(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_02321da4 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, Decoration_Destructor, 0, 0x500, 4, sizeof(UnkStruct_02321da4));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02321da4));
    Decoration_InitFromMap(w, p, a, b);
    ObjRect_SetBox2D(&w->coll0, -0x28, -0x18, 0x28, 0x30);
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unkA |= 0xc0;
    w->coll0.unk1C = DecBird_OnHit;
    w->coll0.unk18 = w;
    StageObj_Load2dBAC(w, data_ov000_02367354, 2, data_ov000_0236a708, data_ov000_02369ba0);
    StageObj_SetOAMOrder(w, 0x16);
    StageObj_SetOAMPriority(w, 2);
    StageObj_AllocSpritePalette(w, 0, 0x1e);
    w->unk58 |= 0x2100;
    if (p->unk2 == 0x37)
        w->unk54 |= 2;
    if (p->unk2 == 0x39)
        w->unk5C ^= 1;
    return w;
}
