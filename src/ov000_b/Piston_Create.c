#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Spr s0;
    UnkStruct_Spr s1;
    void *unk29C;
} UnkStruct_023129a0;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366d64[];
extern u8 data_ov000_0236a16c[];
extern u8 data_ov000_0236a174[];
extern u8 data_ov000_0236a184[];
extern u8 data_ov000_0236a18c[];
extern u8 data_ov000_023661c0[];

void Piston_Destructor(void);
void StageObj_SetSharedSpriteVram(void *w, s32 a, void *b);
s32 ObjVramRef_Acquire(void *a, s32 b, s32 c);
void AnimatorSprite_Init(UnkStruct_Spr *s, void *a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
s32 Sound_AllocHandle(void);
void func_ov000_023127e8(UnkStruct_023129a0 *w);
void func_ov000_02312930(UnkStruct_023129a0 *w);

UnkStruct_023129a0 *Piston_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_023129a0 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, Piston_Destructor, 0, 0xf6, 2, sizeof(UnkStruct_023129a0));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023129a0));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366d64, 0, data_ov000_0236a16c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x31);
    StageObj_SetSharedSpriteVram(w, 0x20, data_ov000_0236a174);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    w->unk29C = data_ov000_0236a184;
    AnimatorSprite_Init(&w->s0, *w->base.unk8C, 0, 0, 0, 0, ObjVramRef_Acquire(data_ov000_0236a184, 0, 8), 0, 0x5000200);
    AnimatorSprite_Init(&w->s1, *w->base.unk8C, 0, 0, 1, 0, ObjVramRef_Acquire(data_ov000_0236a18c, 1, 8), 0, 0x5000600);
    AnimatorSprite_SetAnimation(&w->s0, 1);
    AnimatorSprite_SetAnimation(&w->s1, 1);
    w->s0.unk46 = w->s1.unk46 = 0x14;
    w->s0.unk2C &= ~0x60000;
    w->s0.unk2C |= 0x40000;
    w->s1.unk2C &= ~0x60000;
    w->s1.unk2C |= 0x40000;
    w->s0.unk44 = w->base.spr0.unk44;
    w->s1.unk44 = w->base.spr1.unk44;
    w->s0.unk2C |= 0x10;
    w->s1.unk2C |= 0x10;
    w->base.unk1B8 = w;
    w->base.unk1C8 = data_ov000_023661c0;
    w->base.unk1D8 = 6;
    w->base.unk1DA = 7;
    w->base.unk1D4 = -0x18;
    w->base.unk1D6 = -0x1c;
    w->base.unk58 |= 0x2100;
    w->base.unk54 |= 2;
    w->base.unk1DC = Sound_AllocHandle();
    func_ov000_023127e8(w);
    func_ov000_02312930(w);
    return w;
}
