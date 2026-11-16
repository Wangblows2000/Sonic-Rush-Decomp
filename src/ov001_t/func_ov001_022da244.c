#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022da244;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022da244 spr[31];
    s32 anim[31];
} UnkStruct_022da244;

void func_02033550(UnkSpr_022da244 *s, u16 anim);

#pragma thumb on
void func_ov001_022da244(UnkStruct_022da244 *p, s32 i) {
    s32 k;
    UnkSpr_022da244 *spr;
    s32 *cur;

    k = i + 2;
    spr = &p->spr[k];
    cur = &p->anim[k];
    if (*cur != 2) {
        func_02033550(spr, 2);
        *cur = 2;
    }
    spr->flags &= ~4;
}
#pragma thumb reset
