#include <nitro/types.h>

void func_ov000_02349038(u8 *w);

void func_ov000_02346040(u8 *w) {
    if ((*(u32 *)(w + 0x32c) & 0x800) && *(void **)(w + 0x84) == NULL && (*(u32 *)(w + 0x58) & 4) && (*(u32 *)(w + 0x58) & 8))
        func_ov000_02349038(w);
    if (*(void **)(w + 0x84) != NULL && !(*(u32 *)(w + 0x32c) & 0x1000)) {
        if (((*(u32 *)(w + 0x58) & 1) && (*(u32 *)(w + 0x58) & 2)) || ((*(u32 *)(w + 0x58) & 4) && (*(u32 *)(w + 0x58) & 8))) {
            func_ov000_02349038(w);
            return;
        }
        *(s16 *)(w + 0x3bc) = 0;
        return;
    }
    *(s16 *)(w + 0x3bc) = 0;
}
