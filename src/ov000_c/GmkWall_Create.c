#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_02308eb8;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023661c0[];
extern u8 data_ov000_02366918[];
extern u8 data_ov000_02369f3c[];
extern u8 data_ov000_02369f44[];

void MapObject_Destructor();
void func_ov000_02308e94();

UnkStruct_02308eb8 *GmkWall_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02308eb8 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_02308eb8));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02308eb8));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366918, 0, data_ov000_02369f3c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x20);
    StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_02369f44);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = NULL;
    w->base.coll0.unkA |= 0x800;
    w->base.unk4A = 0x300;
    w->base.unk1B8 = w;
    w->base.unk1C8 = data_ov000_023661c0;
    w->base.unk1D8 = 4;
    w->base.unk1DA = 8;
    w->base.unk1D4 = -0x10;
    w->base.unk1D6 = -0x20;
    w->base.unk58 |= 0x2100;
    w->base.unk8 = w->base.unk14;
    func_ov000_02308e94(w);
    return w;
}
