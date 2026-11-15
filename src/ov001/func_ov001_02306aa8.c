#include <nitro/types.h>

void **func_ov001_0230c090(void);
void *func_ov001_0230f550(void *a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void func_ov001_0231066c(void *a, void *b);

void func_ov001_02306aa8(u8 *p) {
    void *r;
    if (*(s32 *)(p + 0x6c) != 0xa) {
        return;
    }
    r = func_ov001_0230f550(func_ov001_0230c090(), *(s32 *)(p + 0x78), 0, 0, 0, *(u16 *)(p + 0x16), *(s32 *)(p + 0x18), *(s32 *)(p + 0x1c));
    func_ov001_0231066c(*func_ov001_0230c090(), r);
}
