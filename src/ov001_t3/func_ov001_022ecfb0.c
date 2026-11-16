#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSpr_022ecfb0;

typedef struct {
    u8 pad0[0x64];
    UnkSpr_022ecfb0 spr[(0x64c - 0x64) / 0x48];
    u32 anim[10];
} UnkStruct_022ecfb0;

void func_02033550(UnkSpr_022ecfb0 *s, u16 id);
void func_0203344c(UnkSpr_022ecfb0 *s, s32 a, s32 b);
void func_02032d2c(UnkSpr_022ecfb0 *s);

#pragma thumb on
static inline void Digit_022ecfb0(UnkStruct_022ecfb0 *p, u32 d, s16 x, s16 y, s32 prio) {
    UnkSpr_022ecfb0 *s = &p->spr[d];
    u32 *a = &p->anim[d];
    if (*a != d) {
        *a = d;
        func_02033550(s, *a);
    }
    s->x = x;
    s->y = y;
    s->flags &= ~0x60000;
    s->flags |= prio << 17;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}

void func_ov001_022ecfb0(UnkStruct_022ecfb0 *p, s16 x, s16 y, s32 n, s32 prio) {
    u32 hi;
    u32 lo;

    if (n > 99) n = 99;
    hi = n / 10;
    lo = n % 10;
    Digit_022ecfb0(p, hi, x, y, prio);
    x += 9;
    Digit_022ecfb0(p, lo, x, y, prio);
}
#pragma thumb reset
