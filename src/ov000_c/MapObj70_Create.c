#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x212 - 0x20C];
    u16 unk212;
    u8 pad214[0x218 - 0x214];
} UnkStruct_02309e9c;

typedef struct {
    u8 pad0[0x34];
    u32 unk34;
    u16 unk38;
    u8 pad3A[0x98 - 0x3A];
    u32 unk98;
    u16 unk9C;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 data_ov000_0236aad8;

void MapObj70_Destructor(void);
void func_ov000_02309be0(UnkStruct_02309e9c *w);
void func_ov000_02309e40(UnkStruct_02309e9c *w);
void func_ov000_02342780(void);

UnkStruct_02309e9c *MapObj70_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02309e9c *w;
    vu16 zero;

    if (p->unk4 & 0x10) {
        data_ov000_0236aad8.unk38 = data_ov000_0236aad8.unk9C = (u8)p->unk6 * 10000 + (u8)p->unk7 * 100 + p->unk8;
        data_ov000_0236aad8.unk34 |= 0x10;
        data_ov000_0236aad8.unk98 |= 0x10;
        p->unk0 = 0xff;
        return NULL;
    }
    task = Task_Create(StageObj_Main, MapObj70_Destructor, 0, 0x500, 2, sizeof(UnkStruct_02309e9c));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02309e9c));
    MapObject_Init(w, (s32)p, b, c);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = NULL;
    w->base.coll0.unkA |= 0x800;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    w->unk212 = (u8)p->unk6 * 10000 + (u8)p->unk7 * 100 + p->unk8;
    if (p->unk4 & 0x40)
        func_ov000_02309be0(w);
    else
        func_ov000_02309e40(w);
    func_ov000_02342780();
    return w;
}
