#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_0230d2a0;

extern void *data_ov000_02369ba0;
extern s32 data_ov000_0236a00c;
extern u8 data_ov000_02366b20[], data_ov000_0236a010[];

void IronBall_Destructor(void);
void IronBall_OnHit_Unk_ov000_0230ce24(void);
void IronBall_OnHitStartRoll(void);
void IronBall_Action_Idle(UnkStruct_0230d2a0 *w);
void IronBall_CreatePart_ov000_0230cd00(UnkStruct_0230d2a0 *w);

UnkStruct_0230d2a0 *IronBall_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230d2a0 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, IronBall_Destructor, 0, 0x500, 2, sizeof(UnkStruct_0230d2a0));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230d2a0));
    MapObject_Init(w, (s32)p, b, c);
    if (p->unk2 == 0x79) {
        ObjRect_SetBox2D(&w->base.coll0, p->unk6, p->unk7, p->unk8 + p->unk6, p->unk9 + p->unk7);
        ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
        w->base.coll0.unkA |= 0xc0;
        w->base.coll0.unk1C = IronBall_OnHit_Unk_ov000_0230ce24;
        w->base.coll0.unk18 = w;
        w->base.unk58 |= 0x2120;
        return w;
    }
    StageObj_Load2dBAC(w, data_ov000_02366b20, 0x80, data_ov000_0236a010, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0xd);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetBox2D(&w->base.coll0, p->unk6, p->unk7, p->unk8 + p->unk6, p->unk9 + p->unk7);
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unkA |= 0xc0;
    w->base.coll0.unk1C = IronBall_OnHitStartRoll;
    w->base.coll0.unk18 = w;
    w->base.coll1.unkA |= 0x1000;
    if (p->unk4 & 1)
        w->base.coll0.unkA |= 0x1000;
    w->base.unk64 = -0x28;
    w->base.unk66 = 0x28;
    w->base.unk65 = -0x28;
    w->base.unk67 = 0x30;
    w->base.unk58 |= 0x2100;
    w->base.unk58 |= 0x200c0;
    w->base.unk58 |= 0x4000;
    w->base.unk4 = 0;
    w->base.unk3C = 0xc;
    w->base.unk3E = 0xa00;
    w->base.unk40 = 0xa8;
    w->base.unk42 = 0x1e00;
    data_ov000_0236a00c = 0;
    w->base.unk1DC = Sound_AllocHandle();
    IronBall_Action_Idle(w);
    IronBall_CreatePart_ov000_0230cd00(w);
    return w;
}
