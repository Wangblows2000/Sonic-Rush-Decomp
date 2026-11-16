#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x44 - 8];
    u16 pal;
} UnkSprite_022e0884;

typedef struct {
    u8 pad0[0xdc];
    s32 unkdc;
    u8 pade0[0xdc8 - 0xe0];
    UnkSprite_022e0884 spr;
    u8 padx[0xea4 - 0xdc8 - sizeof(UnkSprite_022e0884)];
    u32 anim;
} UnkStruct_022e0884;

void func_02033550(void *s, u16 id);
void func_0203344c(void *s, s32 a, s32 b);
void func_02032d2c(void *s);

#pragma thumb on
void func_ov001_022e0884(UnkStruct_022e0884 *p) {
    UnkSprite_022e0884 *s;
    u32 id;

    s = &p->spr;
    id = 0;
    if (p->unkdc != 0) {
        id++;
    }
    if (id != p->anim) {
        func_02033550(s, id);
        p->anim = id;
    }
    s->pal = 0xe;
    s->x = 0xd8;
    s->y = 0x98;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
