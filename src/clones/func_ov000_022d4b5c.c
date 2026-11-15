#include <nitro/types.h>

s32 func_02025edc(void *p, s32 k);
void func_02025268(s32 a, void *p, s32 n);

BOOL func_ov000_022d4b5c(u8 *p, u32 v, s32 k) {
    s32 r = func_02025edc(*(u8 **)(*(u8 **)(p + 4) + 4) + 0x40, k);
    s32 expect;
    if (*(u32 *)(p + 8) & 0x10) {
        expect = p[0xae];
    } else {
        expect = -1;
    }
    if (r != expect) {
        return FALSE;
    }
    {
        u32 tmp = v;
        func_02025268(0x13, &tmp, 1);
    }
    return TRUE;
}
