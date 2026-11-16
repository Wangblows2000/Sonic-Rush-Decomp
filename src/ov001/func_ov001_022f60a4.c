#include <nitro/types.h>

extern void *data_ov001_0231f068;
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
s32 func_ov001_022fd4ac(void);
void func_ov001_022f3b50(void *p);
void func_ov001_022f5eb4(void);

void func_ov001_022f60a4(u8 *p) {
    u8 *q;
    if (func_ov001_022fd4ac() != 0) {
        func_ov001_022f3b50(p);
        return;
    }
    q = *(u8 **)(p + 0x84);
    *(void (**)(void))(p + 0x74) = func_ov001_022f5eb4;
    *(s32 *)(p + 0x54) = 0;
    *(s32 *)(p + 0x50) = *(s32 *)(p + 0x54);
    Model_SetAnimation(q, 0, data_ov001_0231f068, 5, 0);
    q[0x10c] &= ~4;
    *(s32 *)(q + 0x114) = 0x1000;
}
