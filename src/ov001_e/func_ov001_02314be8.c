#include <nitro/types.h>

typedef struct UnkStruct_02314be8_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02314be8_v;

typedef struct UnkStruct_02314be8_m {
    s32 m[12];
} UnkStruct_02314be8_m;

typedef struct UnkStruct_02314be8_e {
    u8 pad0[0x20];
    UnkStruct_02314be8_v pos;
    UnkStruct_02314be8_v target;
    UnkStruct_02314be8_v up;
    u8 pad44[0x58 - 0x44];
} UnkStruct_02314be8_e;

extern UnkStruct_02314be8_e *data_ov001_0231f6a4;

void func_020044f4(void *a, void *b, void *c, s32 d, void *e);
void func_02025268(s32 a, void *b, s32 c);

void func_ov001_02314be8(s32 idx) {
    UnkStruct_02314be8_e *e = &data_ov001_0231f6a4[idx];
    s32 v;
    UnkStruct_02314be8_m m;

    func_020044f4(&e->pos, &e->up, &e->target, 1, &m);
    v = 2;
    func_02025268(0x10, &v, 1);
    func_02025268(0x15, 0, 0);
    func_02025268(0x17, &m, 0xc);
}
