#include <nitro/types.h>

extern void *data_ov001_0231f068;
extern s32 data_ov001_0231d4c0[];
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void func_ov001_022f3b50(void *p);
u8 *func_ov001_022fb678(void *p);
void func_ov001_022f3c30(void *p);
void func_ov001_022f57a8(void);

void func_ov001_022f5424(u8 *p) {
    u8 *r = *(u8 **)(p + 0x84);
    u8 *q;
    u8 *t;
    func_ov001_022f3b50(p);
    *(s32 *)(p + 0x1c8) -= 1;
    if (*(s32 *)(p + 0x1c8) > 0) {
        return;
    }
    q = *(u8 **)(p + 0x1dc);
    *(void (**)(void))(p + 0x74) = func_ov001_022f57a8;
    Model_SetAnimation(r, 0, data_ov001_0231f068, data_ov001_0231d4c0[*(s32 *)(p + 0x1cc)], 0);
    r[0x10c] &= ~4;
    t = func_ov001_022fb678(p);
    *(u8 **)(q + 0x10) = t;
    t = *(u8 **)(t + 0x84);
    *(u16 *)(t + 8) = 0x1e;
    t[0xa] = 4;
    t[0xb] = 2;
    func_ov001_022f3c30(p);
}
