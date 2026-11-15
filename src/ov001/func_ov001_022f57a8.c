#include <nitro/types.h>

extern void *data_ov001_0231f068;
extern s32 data_ov001_0231d4c0[];
void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);
void func_ov001_022f3b50(void *p);
u8 *func_ov001_022ffce8(void);
s32 func_02002094(s32 a, s32 b);
void func_ov001_022fb5ec(void *p);
void func_ov001_022f56f8(void);

void func_ov001_022f57a8(u8 *p) {
    u8 *r = *(u8 **)(p + 0x84);
    u8 *a;
    u8 *b;
    u8 *c;
    u8 *d;
    func_ov001_022f3b50(p);
    if (!(r[0x10c] & 2)) {
        return;
    }
    *(void (**)(void))(p + 0x74) = func_ov001_022f56f8;
    func_02031a68(r, 0, data_ov001_0231f068, data_ov001_0231d4c0[*(s32 *)(p + 0x1cc)] + 1, 0);
    a = func_ov001_022ffce8();
    b = func_ov001_022ffce8();
    c = func_ov001_022ffce8();
    d = func_ov001_022ffce8();
    *(s32 *)(r + 0x114) = func_02002094(*(s32 *)(a + 0x308) * 0x1333 + (*(s32 *)(b + 0x30c) - *(s32 *)(c + 0x308)) * 0x1800, *(s32 *)(d + 0x30c));
    func_ov001_022fb5ec(*(void **)(*(u8 **)(p + 0x1dc) + 0x10));
    *(s32 *)(p + 0x60) = 0;
    *(s32 *)(p + 0x5c) = *(s32 *)(p + 0x60);
    *(s32 *)(p + 0x54) = *(s32 *)(p + 0x5c);
    *(s32 *)(p + 0x50) = *(s32 *)(p + 0x54);
    *(u32 *)(p + 0xb8) |= 0x200;
}
