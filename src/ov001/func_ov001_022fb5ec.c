#include <nitro/types.h>

void func_ov001_022fd268(void *p, s32 a);

void func_ov001_022fb5ec(u8 *p) {
    *(s32 *)(p + 0x1d0) += 1;
    switch (*(s32 *)(p + 0x1d0)) {
    case 1:
        func_ov001_022fd268(p, 0);
        *(u32 *)(p + 0xf0) |= 0x200;
        break;
    case 2:
        func_ov001_022fd268(p, 1);
        *(u32 *)(p + 0xf0) &= ~0x200;
        break;
    case 3:
        func_ov001_022fd268(p, 0);
        *(u32 *)(p + 0xf0) |= 0x200;
        break;
    }
}
