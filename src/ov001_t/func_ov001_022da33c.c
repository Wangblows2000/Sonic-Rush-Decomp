#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022da33c;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022da33c spr[31];
    s32 anim[31];
} UnkStruct_022da33c;

void func_02033550(UnkSpr_022da33c *s, u16 anim);

#pragma thumb on
void func_ov001_022da33c(UnkStruct_022da33c *p) {
    UnkSpr_022da33c *spr = &p->spr[29];

    if (p->anim[29] != 0) {
        func_02033550(spr, 0);
        p->anim[29] = 0;
    }
    spr->flags &= ~4;
}
#pragma thumb reset
