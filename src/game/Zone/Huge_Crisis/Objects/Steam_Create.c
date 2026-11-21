#include <game/Stage/stageobj_alt.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x2AC - 0x20C];
} UnkStruct_0231a66c;

extern void *data_ov000_02369ba0;
extern void *gPlayers;
extern u8 data_ov000_02367180[], data_ov000_0236a328[];

void MapObject_Destructor(void);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void Sound_SetPositional(s32 a, void *w, void *b);
void Steam_Action_Rise(UnkStruct_0231a66c *w);

UnkStruct_0231a66c *Steam_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231a66c *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_0231a66c));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231a66c));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02367180, 2, data_ov000_0236a328, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0xc);
    w->base.spr0.unk46 = 0x12;
    w->base.spr1.unk46 = 0x12;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -0x10);
    w->base.unk58 |= 0x100;
    StageObj_SetMapHitbox(w, -2, -2, 2, 2);
    w->base.coll0.unk1C = NULL;
    w->base.spr0.unk2C &= ~0x60000;
    w->base.spr1.unk2C &= ~0x60000;
    w->base.unk1DC = Sound_AllocHandle();
    Sound_PlaySeqArcEx(w->base.unk1DC, 0, 0x68);
    Sound_SetPositional(w->base.unk1DC, w, gPlayers);
    Steam_Action_Rise(w);
    return w;
}
