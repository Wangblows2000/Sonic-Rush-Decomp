#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Spr s0;
    UnkStruct_Spr s1;
} UnkStruct_02316698;

typedef struct {
    s32 unk0;
    u8 pad4[0xC];
    s32 unk10;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_02366f48[], data_ov000_0236a244[], data_ov000_02366f60[];

void GmkSpecial_Destructor(void);
void GmkSpecial_Draw(void);
void GmkSpecial_OnHit(void);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void func_ov000_023165b0(UnkStruct_02316698 *w);

UnkStruct_02316698 *GmkSpecial_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02316698 *w;
    vu16 zero;

    if (gGameState.unk10 == 2 || gGameState.unk10 == 1 || gGameState.unk10 == 4 || gGameState.unk0 == 1) {
        p->unk0 = 0xff;
        return NULL;
    }
    task = Task_Create(StageObj_Main, GmkSpecial_Destructor, 0, 0x500, 2, sizeof(UnkStruct_02316698));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02316698));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366f48, 0xffff, data_ov000_0236a244, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 2);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    w->base.unk70 = GmkSpecial_Draw;
    ObjAction_Load2dBAC(&w->s0, &w->s1, data_ov000_02366f60, 4, data_ov000_0236a244, data_ov000_02369ba0);
    AnimatorSprite_SetAnimation(&w->s0, 1);
    AnimatorSprite_SetAnimation(&w->s1, 1);
    w->s0.unk46 = 0x13;
    w->s1.unk46 = 0x13;
    w->s0.unk44 = w->base.spr0.unk44;
    w->s1.unk44 = w->base.spr0.unk44;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -0x10);
    w->base.coll0.unk1C = GmkSpecial_OnHit;
    w->base.coll0.unkA |= 0x800;
    w->base.unk58 |= 0x2100;
    if (Stage_IsGravityFlipped(w->base.unk10, w->base.unk14)) {
        w->base.unk5C |= 2;
        w->base.unk5 = 0x80;
    }
    func_ov000_023165b0(w);
    return w;
}
