#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[4];
    s32 unkC;
    u8 pad10[0x2C - 0x10];
    s32 x;
    s32 y;
    u32 unk34;
    u8 pad38[0xC8 - 0x38];
    u32 flags;
} UnkStruct_02340a4c;

extern UnkStruct_02340a4c gCamera;
extern UnkStruct_02340a4c data_ov000_0236ab3c;

UnkStruct_02340a4c *func_ov000_0233b62c(void);
UnkStruct_02340a4c *func_ov000_0233b610(void);

void func_ov000_02340a4c(void) {
    UnkStruct_02340a4c *a;
    UnkStruct_02340a4c *b;

    if (gCamera.flags & 1) {
        a = func_ov000_0233b62c();
        b = func_ov000_0233b610();
        if (a->unk34 & 1) {
            return;
        }
        a->x += (a->unk4 - a->unkC) / 60;
        a->y = 0x3000;
        b->x += (b->unk4 - b->unkC) / 60;
        b->y = 0x14000;
        return;
    }
    a = &gCamera;
    if (!(a->unk34 & 1)) {
        a->x += (a->unk4 - a->unkC) / 60;
        a->y = 0x3000;
    }
    a = &data_ov000_0236ab3c;
    if (!(a->unk34 & 1)) {
        a->x += (a->unk4 - a->unkC) / 60;
        a->y = 0x3000;
    }
}
