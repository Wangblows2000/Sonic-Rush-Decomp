#include <nitro/types.h>

void func_ov000_02348860(u8 *w);

BOOL func_ov000_02346ddc(u8 *w) {
    if ((*(u32 *)(w + 0x58) & 1) && (*(u16 *)(w + 0x5e) & 8)) {
        if (*(s16 *)(w + 0x328) < 0x1a || *(s16 *)(w + 0x328) > 0x20)
            func_ov000_02348860(w);
        return TRUE;
    }
    return FALSE;
}
