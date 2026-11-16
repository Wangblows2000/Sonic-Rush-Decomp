#include <nitro/types.h>

typedef struct UnkStruct_022f8858_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x44 - 8];
    u16 unk44;
    u8 pad46[2];
} UnkStruct_022f8858_o;

typedef struct UnkStruct_022f8858 {
    u8 pad0[0x18];
    UnkStruct_022f8858_o objs[0x27];
    u8 padB10[0xe38 - 0xb10];
    u32 flags;
    u8 padE3C[0xe4c - 0xe3c];
    s32 unkE4C;
    s32 unkE50;
} UnkStruct_022f8858;

typedef struct UnkStruct_022f8858_t {
    u8 pad0[8];
    void (*fn)(void);
} UnkStruct_022f8858_t;

extern UnkStruct_022f8858 *data_ov001_0231f084;
extern UnkStruct_022f8858_t *data_022b4574;
extern u16 data_ov001_02316aa0[];
extern u16 data_ov001_02316abc[];

void func_ov001_022f87c8(void);
void func_02032d2c(void *p);
void func_02032480(void *o, s32 a, s32 b, s32 c);

void func_ov001_022f8858(void) {
    s32 i;
    s16 x;
    u16 k;
    UnkStruct_022f8858_o *o;
    UnkStruct_022f8858 *g;

    if (data_ov001_0231f084->flags & 3) {
        data_022b4574->fn = func_ov001_022f87c8;
        func_ov001_022f87c8();
        data_ov001_0231f084->flags &= ~0x400;
        data_ov001_0231f084->flags |= 0x800;
        return;
    }
    i = 0;
    x = 0x3d;
    for (; i < data_ov001_0231f084->unkE4C; i++) {
        k = data_ov001_02316aa0[i];
        o = &data_ov001_0231f084->objs[k + 0x18];
        if (k != 0xe) {
            o->x = x;
            o->y = 0x1e;
            o->unk44 = 0;
            func_02032d2c(o);
        }
        x += data_ov001_02316abc[k];
    }
    g = data_ov001_0231f084;
    k = data_ov001_02316aa0[i];
    g->unkE50 += 0x333;
    o = &g->objs[k + 0x18];
    if (data_ov001_0231f084->unkE50 > 0x1000)
        data_ov001_0231f084->unkE50 = 0x1000;
    if (k != 0xe) {
        o->x = x;
        o->y = 0x1e;
        o->unk44 = 0;
        func_02032480(o, 0x1000, data_ov001_0231f084->unkE50, 0);
    }
    if (data_ov001_0231f084->unkE50 < 0x1000)
        return;
    data_ov001_0231f084->unkE50 = 0;
    data_ov001_0231f084->unkE4C++;
    if (data_ov001_0231f084->unkE4C < 0xd)
        return;
    data_022b4574->fn = func_ov001_022f87c8;
    data_ov001_0231f084->flags |= 0x400;
}
