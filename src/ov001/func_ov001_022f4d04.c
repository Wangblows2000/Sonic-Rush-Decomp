#include <nitro/types.h>

extern void *data_ov001_0231f068;
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void func_ov001_022f4cd0(void);

void func_ov001_022f4d04(u8 *p) {
    u8 *q;
    *(s32 *)(p + 0x1c8) -= 1;
    if (*(s32 *)(p + 0x1c8) > 0) {
        return;
    }
    q = *(u8 **)(p + 0x84);
    *(void (**)(void))(p + 0x74) = func_ov001_022f4cd0;
    Model_SetAnimation(q, 0, data_ov001_0231f068, 0x17, 0);
    q[0x10c] &= ~4;
}
