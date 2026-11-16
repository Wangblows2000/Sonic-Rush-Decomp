#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022da304;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022da304 spr[31];
    s32 anim[31];
} UnkStruct_022da304;

void func_02033550(UnkSpr_022da304 *s, u16 anim);

#pragma thumb on
void func_ov001_022da304(UnkStruct_022da304 *p) {
    UnkSpr_022da304 *spr = &p->spr[29];

    if (p->anim[29] != 1) {
        func_02033550(spr, 1);
        p->anim[29] = 1;
    }
    spr->flags &= ~4;
}
#pragma thumb reset
