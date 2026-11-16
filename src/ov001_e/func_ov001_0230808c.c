#include <nitro/types.h>

typedef struct UnkStruct_0230808c_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkStruct_0230808c_o;

typedef struct UnkStruct_0230808c {
    u8 pad0[0x88];
    u8 bg88[0x40];
    u8 bgC8[0x40];
    UnkStruct_0230808c_o objs[0x19];
    u8 pad810[0xbd4 - 0x810];
    s32 unkBD4;
    s32 unkBD8[7];
    void *task;
    s16 posX;
    s16 posY;
    UnkStruct_0230808c_o objs3[3];
} UnkStruct_0230808c;

extern UnkStruct_0230808c *gSpecialStageHud;

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void Background_Draw(void *a);
void AnimatorSprite_SetAnimation(UnkStruct_0230808c_o *s, u16 index);
void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void AnimatorSprite_ProcessAnimation(void *c, u32 a, u32 b);
void func_ov001_02307990(s32 i);
void Sound_PlaySeq(s32 a, s32 b);
void func_ov001_02307ad0(void);

void func_ov001_0230808c(void) {
    UnkStruct_0230808c *g = gSpecialStageHud;
    s32 i;
    vu32 zero;

    g->unkBD4 = 0;
    zero = 0;
    MIi_CpuClear32(zero, (void *)0x6200000, 0x1000);
    Background_Draw(g->bg88);
    Background_Draw(g->bgC8);
    for (i = 0; i < 7; i++) {
        g->unkBD8[i] = 0;
        g->objs[i + 11].x = -100;
        g->objs[i + 11].y = -100;
    }
    g->posX = 0x80;
    g->posY = 0x78;
    AnimatorSprite_SetAnimation(&g->objs[22], 1);
    g->task = Task_Create(func_ov001_02307ad0, 0, 0, 0xf400, 0, 0);
    for (i = 0; i < 3; i++) {
        AnimatorSprite_ProcessAnimation(&g->objs3[i], 0, 0);
        g->objs3[i].flags &= ~0x40;
        func_ov001_02307990(i);
    }
    Sound_PlaySeq(0, 0x1f);
    {
        u32 v = (*(vu32 *)0x4001000 & 0x1f00) >> 8;
        *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | ((v | 2) << 8);
    }
}
