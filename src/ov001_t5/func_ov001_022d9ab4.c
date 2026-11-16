#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkSprite_022d9ab4;

typedef struct {
    u8 pad0[0xc];
    s32 cur;
    u8 pad10[0x210 - 0x10];
    UnkSprite_022d9ab4 spr[30];
} UnkStruct_022d9ab4;

s32 func_02051814(void);
void func_02032d2c(UnkSprite_022d9ab4 *s);

#pragma thumb on
void func_ov001_022d9ab4(UnkStruct_022d9ab4 *p, s16 x, s16 y, BOOL flag, s32 idx) {
    UnkSprite_022d9ab4 *s;

    if (idx < 0) idx = 0;
    if (idx > 3) idx = 3;
    p->cur = func_02051814();
    if (flag) {
        s = &p->spr[idx + 0x19];
    } else {
        s = &p->spr[p->cur + 0x19];
    }
    s->x = x;
    s->y = y;
    func_02032d2c(s);
}
#pragma thumb reset
