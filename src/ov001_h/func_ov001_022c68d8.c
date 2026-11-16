#include <nitro/types.h>

typedef struct UnkStruct_022c68d8_b {
    s32 unk0;
    s32 unk4;
} UnkStruct_022c68d8_b;

typedef struct UnkStruct_022c68d8 {
    u8 pad0[0x10];
    UnkStruct_022c68d8_b *unk10;
} UnkStruct_022c68d8;

extern UnkStruct_022c68d8 *data_022b4574;
void func_0203438c(void *a);
void func_020371c8(void *t);

void func_ov001_022c68d8(void) {
    UnkStruct_022c68d8_b *t = data_022b4574->unk10;
    {
        u32 v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
        *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v | 1) << 8);
    }
    func_0203438c((void *)(t->unk4 + 0x5c + (t->unk0 + 5) * 0x40));
    func_020371c8(data_022b4574);
}
