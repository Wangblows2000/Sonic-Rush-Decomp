#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x210 - 0x20C];
    s32 unk210;
    s32 unk214;
    s32 unk218;
    s8 unk21C;
    s8 unk21D;
    u8 unk21E;
    u8 pad21F;
} UnkStruct_02303e50;

typedef struct {
    u8 pad0[0x3A];
    s8 unk3A;
    u8 pad3B[0x9E - 0x3B];
    s8 unk9E;
    u8 pad9F[0xC8 - 0x9F];
    u32 unkC8;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 gCamera;

void MapObj73_Destructor(void);
void MapObj73_OnHit(void);
void MapObj73_Action_Unk_ov000_02303e24(UnkStruct_02303e50 *w);
void func_ov000_02342780(void);

UnkStruct_02303e50 *MapObj73_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02303e50 *w;
    vu16 zero;
    u32 k;

    k = p->unk4 & 0xf;
    if (k != 1 && k != 2 && k != 4 && k != 8)
        p->unk4 |= 1;
    task = Task_Create(StageObj_Main, MapObj73_Destructor, 0, 0x500, 2, sizeof(UnkStruct_02303e50));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02303e50));
    MapObject_Init(w, (s32)p, b, c);
    ObjRect_SetBox2D(&w->base.coll0, p->unk6, p->unk7, p->unk8 + p->unk6, p->unk9 + p->unk7);
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unkA |= 0xc0;
    w->base.coll0.unk1C = MapObj73_OnHit;
    w->base.coll0.unk18 = w;
    if (gCamera.unkC8 & 1) {
        w->unk21C = gCamera.unk3A;
        w->unk21E = 1;
    } else {
        w->unk21C = gCamera.unk3A;
        w->unk21D = gCamera.unk9E;
        w->unk21E = 2;
    }
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    switch (p->unk4 & 0xf) {
    case 1:
        w->unk218 = 0;
        break;
    case 2:
        w->unk218 = 1;
        break;
    case 4:
        w->unk218 = 2;
        break;
    case 8:
        w->unk218 = 3;
        break;
    }
    w->unk210 = -p->unk6 << 9;
    w->unk214 = -p->unk7 << 9;
    MapObj73_Action_Unk_ov000_02303e24(w);
    func_ov000_02342780();
    return w;
}
