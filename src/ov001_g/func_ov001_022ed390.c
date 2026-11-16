#include <nitro/types.h>

typedef struct UnkSpr_022ed390 {
    u8 pad0[4];
    s16 x;
    s16 y;
} UnkSpr_022ed390;

typedef struct UnkStruct_022ed390 {
    u8 pad0[0x604];
    UnkSpr_022ed390 spr;
    u8 pad[0x69c - 0x60c];
    u32 anim;
} UnkStruct_022ed390;

void func_02033550(UnkSpr_022ed390 *s, u16 anim);
void func_0203344c(UnkSpr_022ed390 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022ed390 *s);

#pragma thumb on
void func_ov001_022ed390(UnkStruct_022ed390 *p, s32 flag) {
    UnkSpr_022ed390 *s = &p->spr;
    u32 *anim = &p->anim;
    u32 a = flag == 0 ? 1 : 0;
    if (*anim != a) {
        *anim = a;
        func_02033550(s, *anim);
    }
    s->x = 0x10;
    s->y = 0x98;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
