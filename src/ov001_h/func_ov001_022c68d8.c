#include <nitro/types.h>

typedef struct UnkStruct_022c68d8_b {
    s32 unk0;
    s32 unk4;
} UnkStruct_022c68d8_b;

typedef struct UnkStruct_022c68d8 {
    u8 pad0[0x10];
    UnkStruct_022c68d8_b *unk10;
} UnkStruct_022c68d8;

extern UnkStruct_022c68d8 *gTaskCurrent;
void Background_Draw(void *a);
void Task_Destroy(void *t);

void func_ov001_022c68d8(void) {
    UnkStruct_022c68d8_b *t = gTaskCurrent->unk10;
    {
        u32 v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
        *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v | 1) << 8);
    }
    Background_Draw((void *)(t->unk4 + 0x5c + (t->unk0 + 5) * 0x40));
    Task_Destroy(gTaskCurrent);
}
