#include <nitro/types.h>

s32 func_ov000_0234cb70();
void func_ov000_0234aa80(u8 *w);

void func_ov000_0234a984(u8 *w) {
    func_ov000_0234cb70(w);
    *(u32 *)(w + 0x58) |= 0x10;
    if (*(s16 *)(w + 0x28) > 0)
        *(s16 *)(w + 0x2e) = -0x180;
    else
        *(s16 *)(w + 0x2e) = 0x180;
    *(s16 *)(w + 0x352) = 0x18;
    func_ov000_0234aa80(w);
}
