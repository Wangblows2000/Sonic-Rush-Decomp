#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSprite_022cbdf0;

typedef struct {
    u8 pad0[0x78];
    UnkSprite_022cbdf0 spr;
    u8 padx[0x390 - 0x78 - sizeof(UnkSprite_022cbdf0)];
    u32 anim;
} UnkStruct_022cbdf0;

void func_02033550(void *s, u16 id);
void func_0203344c(void *s, s32 a, s32 b);
void func_02032d2c(void *s);

#pragma thumb on
void func_ov001_022cbdf0(UnkStruct_022cbdf0 *p, s16 x, s16 y, s32 sel) {
    UnkSprite_022cbdf0 *s;
    u32 *a;
    u32 id;

    s = &p->spr;
    a = &p->anim;
    id = 0;
    if (sel == 1) {
        id++;
    }
    if (id != *a) {
        func_02033550(s, id);
        *a = id;
    }
    s->x = x;
    s->y = y;
    s->flags &= 0x60000;
    s->flags |= 0x60000;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
