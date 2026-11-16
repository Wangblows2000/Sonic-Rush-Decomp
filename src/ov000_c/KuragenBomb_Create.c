#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x240 - 0x20C];
} UnkStruct_02301258;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366670[];
extern u8 data_ov000_02369e3c[];

void KuragenBomb_OnHit_Box1();
void KuragenBomb_Destructor();

UnkStruct_02301258 *KuragenBomb_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02301258 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, KuragenBomb_Destructor, 0, 0x501, 2, sizeof(UnkStruct_02301258));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02301258));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_SetMapHitbox(w, -6, -4, 6, 4);
    StageObj_Load2dBAC(w, data_ov000_02366670, 2, data_ov000_02369e3c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x1a);
    StageObj_SetOAMOrder(w, 0x15);
    ObjRect_SetAtkDefFlags(&w->base.coll1, 0x13, -16);
    w->base.coll1.unk1C = KuragenBomb_OnHit_Box1;
    w->base.unk58 |= 0x100;
    w->base.unk1DC = Sound_AllocHandle();
    return w;
}
