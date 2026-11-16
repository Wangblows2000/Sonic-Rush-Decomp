#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x2A4 - 0x20C];
    s32 unk2A4;
    u8 pad2A8[4];
} UnkStruct_0231af74;

typedef struct {
    s16 a;
    s16 b;
    s16 c;
    s16 d;
} UnkStruct_02364454;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023671d0[];
extern u8 data_ov000_0236a330[];
extern u8 data_ov000_0236a338[];
extern UnkStruct_02364454 data_ov000_02364454[];

void func_ov000_0231af54(void);
void func_ov000_0231aa74(UnkStruct_0231af74 *w);
void func_ov000_0231af10(UnkStruct_0231af74 *w);

UnkStruct_0231af74 *func_ov000_0231af74(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231af74 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_0231af54, 0, 0x500, 2, sizeof(UnkStruct_0231af74));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231af74));
    func_ov000_022fba50(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_023671d0, 0, data_ov000_0236a330, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x31);
    StageObj_SetSharedSpriteVram(w, 0x20, data_ov000_0236a338);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -0x10);
    w->base.coll0.unk1C = NULL;
    w->base.coll0.unkA |= 0x800;
    w->base.unk58 |= 0x2100;
    switch (w->base.unk1E0->unk2) {
    default:
    case 0xa2:
        w->unk2A4 = 0;
        w->base.unk5C &= ~2;
        break;
    case 0xa3:
        w->unk2A4 = 1;
        w->base.unk5C |= 2;
        break;
    case 0xa4:
        w->unk2A4 = 2;
        w->base.unk5C |= 1;
        w->base.unk5C |= 0x100;
        w->base.unk1 = 0xc0;
        break;
    case 0xa5:
        w->unk2A4 = 3;
        w->base.unk5C &= ~1;
        w->base.unk5C |= 0x100;
        w->base.unk1 = 0x40;
        break;
    }
    StageObj_SetAnimation(w, 0);
    w->base.unk5C |= 4;
    ObjRect_SetBox2D(&w->base.coll1, data_ov000_02364454[w->unk2A4].a, data_ov000_02364454[w->unk2A4].b,
                  data_ov000_02364454[w->unk2A4].c, data_ov000_02364454[w->unk2A4].d);
    ObjRect_SetAtkDefFlags(&w->base.coll1, 0x13, 0x13);
    w->base.coll1.unk1C = NULL;
    w->base.coll1.unk18 = w;
    w->base.unk54 |= 2;
    func_ov000_0231aa74(w);
    func_ov000_0231af10(w);
    return w;
}
