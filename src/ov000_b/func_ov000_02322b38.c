#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x58];
    u32 unk58;
    u8 pad5C[0x94 - 0x5C];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
    UnkStruct_Coll coll0;
    u8 pad160[0x19C - 0x160];
} UnkStruct_02322b38;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_0236739c[], data_ov000_0236a6f8[], data_ov000_0236a840[], data_ov000_0236a850[];

void func_ov000_023265d8(void);
void func_ov000_02322acc(void);
void func_ov000_02326638(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);

UnkStruct_02322b38 *func_ov000_02322b38(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_02322b38 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_023265d8, 0, 0x500, 4, sizeof(UnkStruct_02322b38));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02322b38));
    func_ov000_02326638(w, p, a, b);
    ObjRect_SetBox2D(&w->coll0, -8, -0x60, 8, -0x20);
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unkA |= 0xc0;
    w->coll0.unk1C = func_ov000_02322acc;
    w->coll0.unk18 = w;
    StageObj_Load2dBAC(w, data_ov000_0236739c, 0, data_ov000_0236a6f8, data_ov000_02369ba0);
    if (p->unk2 < 0x33) {
        StageObj_AllocSpritePalette(w, 0, 5);
        StageObj_SetSharedSpriteVram(w, 9, data_ov000_0236a840);
        StageObj_SetAnimation(w, 6);
        StageObj_SetOAMPriority(w, 3);
    } else {
        StageObj_AllocSpritePalette(w, 7, 0x16);
        StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_0236a850);
        StageObj_SetAnimation(w, 7);
        StageObj_SetOAMPriority(w, 2);
    }
    if (!(p->unk2 & 1)) {
        StageObj_SetOAMOrder(w, 6);
        w->spr0.unk2C -= 0x40000;
        w->spr1.unk2C -= 0x40000;
    }
    w->unk58 |= 0x2100;
    return w;
}
