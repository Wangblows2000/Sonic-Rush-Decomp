#include <nitro/types.h>

void func_ov001_022fa558(void *p);
u32 *func_ov001_022ffce8(void);

void func_ov001_022fa5c8(u8 *p) {
    func_ov001_022fa558(p);
    if (!(*func_ov001_022ffce8() & 0x40)) {
        *(u32 *)(p + 0x14) |= 8;
    }
}
