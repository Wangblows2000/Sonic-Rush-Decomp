#include <nitro/types.h>

typedef struct UnkStruct_02315714_m {
    s32 m[16];
} UnkStruct_02315714_m;

void func_0200468c(s32 t, s32 b, s32 l, s32 r, s32 n, s32 f, s32 s, s32 z, UnkStruct_02315714_m *m);
void func_02025268(s32 a, void *b, s32 c);

void func_ov001_02315714(void) {
    s32 a;
    s32 b;
    UnkStruct_02315714_m m;

    func_0200468c(0, 0xc00, 0, 0x1000, -0x1000, 0x1000, 0x1000, 0, &m);
    a = 0;
    func_02025268(0x10, &a, 1);
    func_02025268(0x11, 0, 0);
    func_02025268(0x15, 0, 0);
    func_02025268(0x16, &m, 0x10);
    b = 2;
    func_02025268(0x10, &b, 1);
    func_02025268(0x11, 0, 0);
    func_02025268(0x15, 0, 0);
}
