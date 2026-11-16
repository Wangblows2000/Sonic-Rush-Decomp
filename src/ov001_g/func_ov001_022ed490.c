#include <nitro/types.h>

typedef struct UnkSpr_022ed490 {
    u8 pad0[4];
    s16 x;
    s16 y;
} UnkSpr_022ed490;

typedef struct UnkStruct_022ed490 {
    u8 pad0[0x5bc];
    UnkSpr_022ed490 spr;
    u8 pad[0x698 - 0x5c4];
    u32 anim;
} UnkStruct_022ed490;

void func_02033550(UnkSpr_022ed490 *s, u16 anim);
void func_0203344c(UnkSpr_022ed490 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022ed490 *s);

#pragma thumb on
void func_ov001_022ed490(UnkStruct_022ed490 *p, s16 x, s16 y) {
    UnkSpr_022ed490 *s = &p->spr;
    u32 *anim = &p->anim;
    if (*anim != 0) {
        *anim = 0;
        func_02033550(s, *anim);
    }
    s->x = x;
    s->y = y;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
