#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkSprite_022cbecc;

typedef struct {
    u8 pad0[0x78];
    UnkSprite_022cbecc spr[10];
    u8 padx[0x390 - 0x78 - 10 * 0x48];
    u32 anim[10];
} UnkStruct_022cbecc;

void func_02033550(void *s, u16 id);
void func_0203344c(void *s, s32 a, s32 b);
void func_02032d2c(void *s);

#pragma thumb on
void func_ov001_022cbecc(UnkStruct_022cbecc *p, s32 idx, s16 x, s16 y, s32 sel) {
    UnkSprite_022cbecc *s;
    u32 *a;
    u32 id;
    s32 n;

    n = idx + 2;
    s = &p->spr[n];
    a = &p->anim[n];
    id = idx * 2 + 1;
    if (sel == 1) {
        id++;
    }
    if (id != *a) {
        func_02033550(s, id);
        *a = id;
    }
    s->x = x;
    s->y = y;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
