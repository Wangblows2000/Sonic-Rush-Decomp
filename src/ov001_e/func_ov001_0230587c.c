#include <nitro/types.h>

typedef struct UnkStruct_0230587c_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_0230587c_o;

typedef struct UnkStruct_0230587c {
    u32 flags;
    u8 pad4[0x14 - 4];
    s32 unk14;
    u8 pad18[0x98 - 0x18];
    UnkStruct_0230587c_o objs[8];
    u8 pad2D8[0x5e8 - 0x2d8];
    s32 timer;
} UnkStruct_0230587c;

extern UnkStruct_0230587c *data_ov001_0231f5a8;
extern void *data_022b4574;
extern u32 data_0208fe70;

s32 func_0200203c(s32 n, s32 d);
void func_020371c8(void *task);
void func_02032d2c(void *p);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return data_0208fe70 >> 16;
}

void func_ov001_0230587c(void) {
    s16 x;
    s16 y;
    s32 i;
    s32 r;

    data_ov001_0231f5a8->timer--;
    if (data_ov001_0231f5a8->timer > 0) {
        r = data_ov001_0231f5a8->timer * 0x500;
        x = ((func_0200203c(Rand(), r) - (r >> 1)) >> 12) + 0x80;
        y = ((func_0200203c(Rand(), r) - (r >> 1)) >> 12) + 0x5b;
    } else {
        func_020371c8(data_022b4574);
        x = 0x80;
        data_ov001_0231f5a8->unk14 = 0;
        y = 0x5b;
        data_ov001_0231f5a8->flags |= 2;
    }
    for (i = 4; i <= 7; i++) {
        UnkStruct_0230587c_o *o = &data_ov001_0231f5a8->objs[i];
        o->x = x;
        o->y = y;
        func_02032d2c(o);
    }
}
