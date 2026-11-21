#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2C];
    u32 flags;
    u8 pad30[0x44 - 0x30];
    u16 unk44;
    u16 unk46;
} UnkStruct_0235ad8c_Spr;

typedef struct {
    s32 unk0;
    s32 unk4;
    UnkStruct_0235ad8c_Spr spr[2];
} UnkStruct_0235ad8c;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_0235ad8c *work;
} UnkStruct_0235ad8c_Task;

UnkStruct_0235ad8c_Task *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void *OS_GetArenaLo(u32 arg);
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void AnimatorSprite_Init(UnkStruct_0235ad8c_Spr *s, void *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, u32 h);
void RingEffect2D_Main(void);

void RingEffect2D_Create(s32 a, s32 b, void *res, void *f0, void *f1) {
    UnkStruct_0235ad8c_Task *t;
    UnkStruct_0235ad8c *w;
    vu16 zero;

    t = Task_Create(RingEffect2D_Main, NULL, 0, 0x3000, 2, sizeof(UnkStruct_0235ad8c));
    if (t == OS_GetArenaLo(0))
        return;
    w = t->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0235ad8c));
    w->unk0 = a;
    w->unk4 = b;
    AnimatorSprite_Init(&w->spr[0], res, 1, 0, 0, 0, f0, 0, 0x5000200);
    AnimatorSprite_Init(&w->spr[1], res, 1, 0, 1, 0, f1, 0, 0x5000600);
    w->spr[0].unk44 = 2;
    w->spr[1].unk44 = 2;
    w->spr[0].flags |= 0x24010;
    w->spr[1].flags |= 0x24010;
}
