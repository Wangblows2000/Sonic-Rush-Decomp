#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_0231f3ec_V;

typedef struct {
    u8 pad0[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_0231f3ec_M;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x124 - 0x1C];
    UnkStruct_0231f3ec_M *unk124;
} UnkStruct_0231f3ec_O;

typedef struct {
    UnkStruct_0231f3ec_O *ox;
    UnkStruct_0231f3ec_O *oy;
    UnkStruct_0231f3ec_O *oz;
    s32 offx;
    s32 offy;
    s32 offz;
    s32 x;
    s32 y;
    s32 z;
    u8 pad24[0x3C - 0x24];
} UnkStruct_0231f3ec_T;

typedef struct {
    u32 unk0;
    u8 pad4[0x18 - 4];
    UnkStruct_0231f3ec_V unk18;
    u8 pad24[0x3C - 0x24];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    u8 pad48[0xDC - 0x48];
    s32 unkDC;
    u8 unkE0[0x128 - 0xE0];
    UnkStruct_0231f3ec_V unk128;
} UnkStruct_0231f3ec;

extern UnkStruct_0231f3ec_T data_ov000_0236a48c[];
extern UnkStruct_0231f3ec data_ov000_0236a444;
extern void (*data_ov000_023644d4[])(void);

void func_020316f4(void *a);
void func_020315e0(void *a);

static inline void Track(UnkStruct_0231f3ec *c, UnkStruct_0231f3ec_T *t, u32 mask) {
    if (t != NULL) {
        if (c->unk0 & mask) {
            if (t->ox != NULL)
                t->x = t->ox->unk124->unk48 + t->offx;
            if (t->oy != NULL)
                t->y = t->oy->unk124->unk4C + t->offy;
            if (t->oz != NULL)
                t->z = t->oz->unk124->unk50 + t->offz;
        } else {
            if (t->ox != NULL)
                t->x = (t->ox->unk10 << 4) + t->offx;
            if (t->oy != NULL)
                t->y = t->offy - (t->oy->unk14 << 4);
            if (t->oz != NULL)
                t->z = (t->oz->unk18 << 4) + t->offz;
        }
    }
}

void func_ov000_0231f3ec(void) {
    UnkStruct_0231f3ec *c = &data_ov000_0236a444;

    UnkStruct_0231f3ec_T *t = data_ov000_0236a48c;
    Track(c, &t[0], 2);
    Track(c, &t[1], 4);
    data_ov000_023644d4[c->unkDC]();
    if (c->unk0 & 1) {
        c->unk128 = c->unk18;
        func_020316f4(c->unkE0);
        func_020315e0(c->unkE0);
    }
    c->unk3C = c->unk40 = c->unk44 = 0;
}
