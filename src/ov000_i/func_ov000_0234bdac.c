#include <nitro/types.h>

void func_ov000_0234bc30(u8 *w);
void func_ov000_0234c4a4(u8 *w, s32 a);
void func_ov000_0234bce4(void);

void func_ov000_0234bdac(u8 *w) {
    s16 v = *(s16 *)(w + 0x34);

    if (v > 0x80 || v < -0x80) {
        func_ov000_0234bc30(w);
        return;
    }
    func_ov000_0234c4a4(w, 3);
    *(u32 *)(w + 0x58) &= ~0x10;
    *(void **)(w + 0x68) = func_ov000_0234bce4;
    *(s16 *)(w + 0x34) = 0;
    *(s16 *)(w + 0x2e) = 0;
}
