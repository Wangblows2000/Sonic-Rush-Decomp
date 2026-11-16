#include <nitro/types.h>

typedef struct {
    u16 a;
    u16 d;
    s32 c;
    u16 n;
    u16 pad;
} UnkStruct_0233e8ac;

void MIi_CpuClear16(u16 data, void *dest, u32 size);

void func_ov000_0233e8ac(UnkStruct_0233e8ac *p, s32 n, s32 c) {
    vu16 zero;
    u16 i;
    u16 a;
    u16 d;

    zero = 0;
    MIi_CpuClear16(zero, p, sizeof(UnkStruct_0233e8ac));
    a = c << 9;
    d = 0x2000;
    for (i = 0; i < n; i++) {
        a -= d;
        if (i % 8 == 0)
            d -= d / 10;
    }
    p->c = c;
    p->a = a;
    p->d = d;
    p->n = n;
}
