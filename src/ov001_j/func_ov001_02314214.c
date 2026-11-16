#pragma opt_lifetimes off
#include <nitro/types.h>

typedef struct UnkStruct_02314214_t {
    u8 pad0[0x14];
    s32 start;
    s32 end;
} UnkStruct_02314214_t;

typedef struct UnkStruct_02314214 {
    u8 pad0[0xc];
    s32 unkC;
    u8 pad10[4];
    UnkStruct_02314214_t *t;
} UnkStruct_02314214;

extern UnkStruct_02314214 *data_ov001_0231f69c;

s32 func_ov001_0230d6b8(void);
s32 *func_ov001_0230d6cc(void);
s32 func_0200225c(s32 n, s32 d);
BOOL func_0202e564(float a, float b);

s32 func_ov001_02314214(void) {
    UnkStruct_02314214 *g = data_ov001_0231f69c;
    float v;
    s32 n;
    s32 r;

    v = (float)g->unkC + (float)func_0200225c(func_ov001_0230d6b8(), 0x12c000) / 4096.0f;
    if (func_0202e564(v, 2.0f))
        return 0;
    if (*func_ov001_0230d6cc() == 5)
        return 0x800;
    if (*func_ov001_0230d6cc() == 6)
        return 0x1000;
    {
        UnkStruct_02314214_t *t = g->t;
        n = t->start;
        if (func_0202e564(v, n - 1))
            return 2048.0f * (v - 2.0f) / (n - 5);
        n = t->start;
        r = (s32)(2048.0f * (v - (n + 3)) / (t->end - n - 6)) + 0x800;
    }
    if (r > 0x1000)
        r = 0x1000;
    return r;
}
