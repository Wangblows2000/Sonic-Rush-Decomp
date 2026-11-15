#include <nitro/types.h>

BOOL func_ov001_02312138(void *p);
void func_ov001_023122d8(void *p, s32 v);
void func_02036754(s32 a, s32 b, s32 c);

void func_ov001_023118d4(u8 *p) {
    u8 *q = *(u8 **)(p + 0x118);
    BOOL b;
    if (q == NULL) {
        b = FALSE;
    } else {
        if (*(s32 *)(q + 0x28) > 0) {
            b = FALSE;
        } else {
            b = TRUE;
        }
    }
    if (!b) {
        if (func_ov001_02312138(p)) {
            *(s32 *)(p + 0x18) = 0;
            func_ov001_023122d8(p, 2);
        } else {
            func_ov001_023122d8(p, 8);
            func_02036754(0, 0x10, 6);
        }
        *(s32 *)(p + 0x124) = 0;
        return;
    }
    *(s32 *)(p + 0x124) = 1;
}
