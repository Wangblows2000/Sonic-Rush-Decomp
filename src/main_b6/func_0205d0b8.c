#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_0205d0b8;

typedef struct {
    u32 pad0;
    u8 *str;
    u16 count;
    u16 pada;
    s32 x;
    s32 y;
    s32 spacing;
} UnkText_0205d0b8;

typedef struct {
    u8 pad0[0x1c];
    u8 zone;
} UnkState_0205d0b8;

extern UnkState_0205d0b8 data_022c4560;
extern u8 data_02076b80[];

void func_02032d2c(UnkSpr_0205d0b8 *s);

void func_0205d0b8(u8 *w, UnkText_0205d0b8 *t) {
    s32 x = 0;
    u32 i;

    if (data_022c4560.zone >= 7)
        return;
    for (i = 0; i < t->count; i++) {
        u8 c = t->str[i];
        if (c < 14) {
            UnkSpr_0205d0b8 *s = &((UnkSpr_0205d0b8 *)(w + 0x4dc))[c];
            s->x = (t->x + x) >> 12;
            s->y = t->y >> 12;
            func_02032d2c(s);
        }
        x += t->spacing * data_02076b80[t->str[i]];
    }
}
