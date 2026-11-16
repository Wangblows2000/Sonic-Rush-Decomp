#include <nitro/types.h>

extern s32 data_ov000_02369b18[];
extern u16 data_ov000_02363070[];
extern u16 data_ov000_02363074[];
extern u32 data_ov000_02365414[];
extern u16 data_ov000_0236306c[];

u8 *func_02025ecc(s32 p);
void func_02022228(s32 a, s32 b, s32 c);
void func_02025e1c(void *p, s32 a, s32 b);
void func_ov000_022c5d4c(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void func_ov000_022dad70(s32 k, BOOL f) {
    u8 *r = func_02025ecc(data_ov000_02369b18[2]);
    u8 *q = r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc);
    u16 *t;
    if (f) {
        func_02022228(0x2108, 0x2108, 0);
        func_02025e1c(q, 0, 0x400);
        t = &data_ov000_02363070[k];
    } else {
        func_02025e1c(q, 1, 0x400);
        t = &data_ov000_02363074[k];
    }
    func_ov000_022c5d4c(data_ov000_02369b18[2], data_ov000_02365414, t, data_ov000_0236306c, 1);
}
