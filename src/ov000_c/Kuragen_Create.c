#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Coll unk20C;
    u32 unk238;
    void *unk23C;
} UnkStruct_023018cc;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366690[], data_ov000_02369e34[], data_ov000_023666b0[], data_ov000_02369e2c[];

void MapObject_Destructor(void);
void func_ov000_02301488(void);
void func_ov000_02301390(UnkStruct_023018cc *w);
void Kuragen_Action_Unk_ov000_0230187c(UnkStruct_023018cc *w);

UnkStruct_023018cc *Kuragen_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_023018cc *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_023018cc));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023018cc));
    MapObject_Init(w, (s32)p, b, c);
    func_02068e40(w, 0xc, -12, 0xc, 0x14);
    switch (w->base.unk1E0->unk2) {
    case 5:
        StageObj_Load2dBAC(w, data_ov000_02366690, 0x12, data_ov000_02369e34, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x1a);
        break;
    case 6:
        StageObj_Load2dBAC(w, data_ov000_023666b0, 0x14, data_ov000_02369e2c, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x1b);
        break;
    }
    StageObj_SetOAMOrder(w, 0x14);
    ObjRect_SetBox2D(&w->unk20C, -0x40, 0, 0x40, 0x170);
    w->unk20C.unkA |= 0xc0;
    ObjRect_SetAtkDefFlags(&w->unk20C, 0, -16);
    w->unk20C.unk28 = 0x12;
    w->unk20C.unk1C = func_ov000_02301488;
    w->unk20C.unk18 = w;
    if (w->base.unk1E0->unk2 == 5) {
        w->unk238 |= 1;
        func_ov000_02301390(w);
    } else {
        w->base.unkC = 0xb4;
        w->unk23C = w->base.coll0.unk1C;
    }
    w->base.unk58 |= 0x100;
    Kuragen_Action_Unk_ov000_0230187c(w);
    return w;
}
