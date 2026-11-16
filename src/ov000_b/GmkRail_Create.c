#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02366d94[], data_ov000_0236a1ac[], data_ov000_02366dac[];

void MapObject_Destructor(void);
void GmkRail_OnHit(void);
s32 FX_Atan2(s32 y, s32 x);
void func_ov000_02313310(UnkStruct_StageObj *w);

UnkStruct_StageObj *GmkRail_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, (s32)p, b, c);
    if (data_022c4560.unk1C == 5) {
        StageObj_Load2dBAC(w, data_ov000_02366d94, 5, data_ov000_0236a1ac, data_ov000_02369ba0);
        StageObj_SetAnimation(w, 4);
    } else {
        StageObj_Load2dBAC(w, data_ov000_02366dac, 5, data_ov000_0236a1ac, data_ov000_02369ba0);
        StageObj_SetAnimation(w, 1);
    }
    StageObj_AllocSpritePalette(w, 0, 0x11);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    ObjRect_SetBox2D(&w->coll0, 4, -8, 0x10, 0);
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unk1C = GmkRail_OnHit;
    w->coll0.unkA |= 0x800;
    w->coll0.unk18 = w;
    w->unk58 |= 0x100;
    w->unk8 = (w->unk1E8 + (p->unk8 + (p->unk6 << 6))) << 8;
    w->unkC = FX_Atan2(p->unk9 + (p->unk7 << 6), p->unk8 + (p->unk6 << 6)) << 8;
    func_ov000_02313310(w);
    return w;
}
