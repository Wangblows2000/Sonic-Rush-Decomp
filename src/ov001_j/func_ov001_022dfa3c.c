#pragma opt_unroll_loops off
#include <nitro/types.h>

typedef struct UnkStruct_022dfa3c_v {
    s32 x, y, z;
} UnkStruct_022dfa3c_v;

typedef struct UnkStruct_022dfa3c_c {
    u8 pad0[0x14];
    UnkStruct_022dfa3c_v pos;
    UnkStruct_022dfa3c_v up;
    UnkStruct_022dfa3c_v target;
    s32 unk38;
    s32 unk3C;
} UnkStruct_022dfa3c_c;

typedef struct UnkStruct_022dfa3c_w {
    u8 pad0[0xbd8];
    s32 unkBD8;
} UnkStruct_022dfa3c_w;

typedef struct UnkStruct_022dfa3c_d38 {
    u8 pad0[0x4c];
    u8 unk4C[0xfc - 0x4c];
    u32 unkFC;
    u8 pad100[0x240 - 0x100];
    UnkStruct_022dfa3c_v pos;
    UnkStruct_022dfa3c_v target;
    UnkStruct_022dfa3c_v up;
} UnkStruct_022dfa3c_d38;

extern UnkStruct_022dfa3c_v data_02084f78;
extern UnkStruct_022dfa3c_v data_02084f84;
extern UnkStruct_022dfa3c_v data_02084f90;
extern u8 data_02084d84[];
extern UnkStruct_022dfa3c_d38 data_02084d38;

s32 func_0200225c(s32 n, s32 d);
void func_020044f4(void *a, void *b, void *c, s32 d, void *e);

void func_ov001_022dfa3c(UnkStruct_022dfa3c_w *w, UnkStruct_022dfa3c_c *c) {
    s64 k = c->unk38;
    s32 lim = c->unk3C;
    s32 v = w->unkBD8;
    s32 a;
    s32 b;

    a = func_0200225c((s32)((v * k + 0x800) >> 12), 0x19000);
    b = func_0200225c(v + (s32)((k * 0x19000 + 0x800) >> 12), 0x1000 - a);
    if (v > lim) {
        c->up.y = v;
    } else if (v > -lim) {
        c->up.y = lim;
    } else {
        c->up.y = b;
    }
    data_02084d38.pos = c->pos;
    data_02084d38.target = c->target;
    data_02084d38.up = c->up;
    func_020044f4(&c->pos, &c->target, &c->up, 0, data_02084d38.unk4C);
    data_02084d38.unkFC &= ~0xe8;
}
