#include <nitro/types.h>

extern void *data_ov001_0231f068;
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void func_ov001_022f5954(void);

void func_ov001_022f5984(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    s32 v = *(s32 *)(q + 0x114);
    if (v > 0) {
        if (**(s32 **)(q + 0xe4) + v < 0x64000) {
            return;
        }
    } else {
        if (**(s32 **)(q + 0xe4) + v > 0x64000) {
            return;
        }
    }
    *(void (**)(void))(p + 0x74) = func_ov001_022f5954;
    Model_SetAnimation(q, 0, data_ov001_0231f068, 7, 0);
    q[0x10c] &= ~4;
    *(s32 *)(q + 0x114) = 0x1000;
}
