#include <nitro/types.h>

void func_ov000_023457c8(u8 *w, s32 a);
BOOL func_ov000_023470ec(u8 *w);
void func_ov000_0234ab34(u8 *w);
void func_ov000_0234bc30(u8 *w);
void func_ov000_0234ba48(u8 *w);

void func_ov000_0234bce4(u8 *w) {
    s16 v;

    if (((w[1] + 0x30) & 0xff) >= 0x60)
        func_ov000_023457c8(w, 0);
    if (func_ov000_023470ec(w)) {
        func_ov000_0234ab34(w);
        return;
    }
    v = *(s16 *)(w + 0x34);
    if (v > 0x80 || v < -0x80) {
        func_ov000_0234bc30(w);
        return;
    }
    if (v == 0 && *(s16 *)(w + 0x2e) == 0 && (*(u16 *)(w + 0x422) & 3)) {
        func_ov000_0234ba48(w);
        return;
    }
    if (!(*(u16 *)(w + 0x420) & 0x80))
        (*(void (**)(u8 *))(w + 0x334))(w);
}
