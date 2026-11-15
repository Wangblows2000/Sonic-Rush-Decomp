#include <nitro/types.h>

void func_ov001_022f12c0(void);

void func_ov001_022f1660(u8 *p) {
    if (*(u16 *)(*(u8 **)(p + 0x1dc) + 0x14) == 0) {
        *(void (**)(void))(p + 0x74) = func_ov001_022f12c0;
    }
}
