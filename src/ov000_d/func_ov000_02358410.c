#include <nitro/types.h>

typedef struct {
    s32 m[4];
} UnkMtx22_02358410;

extern u8 data_02089264[];

s32 func_0200225c(s32 n, s32 d);
void func_02000ccc(UnkMtx22_02358410 *m, s32 sx, s32 sy);
void func_02003d94(void *p, UnkMtx22_02358410 *m, s32 x, s32 y, s32 a, s32 b);

#pragma thumb on

void func_ov000_02358410(void *w, s32 scale) {
    UnkMtx22_02358410 m;
    s32 s = func_0200225c(0x1000, scale);

    func_02000ccc(&m, s, s);
    func_02003d94(data_02089264, &m, 0x80, 0x58, 0, 0);
}

#pragma thumb reset
