#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    u16 unk214;
    u8 unk216;
    u8 pad217;
} UnkStruct_02302774;

typedef struct {
    u32 unk0;
    u8 pad4[0x62 - 4];
    u16 unk62;
} UnkStruct_Plr;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366700[], data_ov000_02369e54[];
extern UnkStruct_Plr gCamera;
extern UnkStruct_Plr data_ov000_0236ab3c;

void MapObject_Destructor(void);
void Bubble_OnHit(void);
void Bubble_Action_Unk_ov000_02302418(UnkStruct_02302774 *w);
void func_ov000_02302694(UnkStruct_02302774 *w);

UnkStruct_02302774 *Bubble_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02302774 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_02302774));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02302774));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366700, 8, data_ov000_02369e54, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x1c);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -0x10);
    w->base.unk58 |= 0x100;
    switch (w->base.unk1E0->unk2) {
    case 0xe8:
        func_02068e40(w, -10, -10, 10, 10);
        w->unk216 = 10;
        w->base.coll0.unk1C = Bubble_OnHit;
        w->base.coll0.unkA |= 0x800;
        break;
    case 0xe9:
        func_02068e40(w, -2, -2, 2, 2);
        w->unk216 = 2;
        w->base.coll0.unk1C = NULL;
        break;
    case 0xea:
        func_02068e40(w, -1, -1, 1, 1);
        w->unk216 = 1;
        w->base.coll0.unk1C = NULL;
        break;
    }
    if ((&gCamera)[2].unk0 & 1)
        w->unk214 = gCamera.unk62;
    else if (gCamera.unk0 & 0x1000000)
        w->unk214 = gCamera.unk62;
    else {
        UnkStruct_Plr *q = (UnkStruct_Plr *)(u32)&data_ov000_0236ab3c;
        if (q->unk0 & 0x1000000)
            w->unk214 = q->unk62;
        else
            w->unk214 = 0;
    }
    if (w->base.unk1E0->unk2 == 0xe8)
        Bubble_Action_Unk_ov000_02302418(w);
    else
        func_ov000_02302694(w);
    return w;
}
