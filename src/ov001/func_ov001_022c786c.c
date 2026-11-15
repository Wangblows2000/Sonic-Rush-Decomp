#include <nitro/types.h>

void func_ov001_022c6ac4(void *p, s32 a, s32 b, s32 c, s32 d);
BOOL func_ov001_022c69ac(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov001_022c7a34(void *p, void (*f)(void));
void func_ov001_022c77e4(void);

void func_ov001_022c786c(u8 *p) {
    if (*(s32 *)(p + 0xc) >= 5) {
        func_ov001_022c6ac4(p, *(s32 *)(p + 0xc) - 5, 0x3c, 1, 0);
    }
    if (*(s32 *)(p + 0xc) < 0x1a9) {
        return;
    }
    if (!func_ov001_022c69ac(p, *(s32 *)(p + 0xc) - 0x1a9, 0x3c, 1, 0)) {
        return;
    }
    func_ov001_022c7a34(p, func_ov001_022c77e4);
}
