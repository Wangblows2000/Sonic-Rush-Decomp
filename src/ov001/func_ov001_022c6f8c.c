#include <nitro/types.h>

extern u16 gDisplayMain[];
void func_ov001_022c6ac4(void *p, s32 a, s32 b, s32 c, s32 d);
BOOL func_ov001_022c69ac(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov001_022c7a34(void *p, void (*f)(void));
void Task_Create(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov001_022c6ef4(void);
void func_ov001_022c6bd4(void);

void func_ov001_022c6f8c(u8 *p) {
    if (*(s32 *)(p + 0xc) >= 5) {
        func_ov001_022c6ac4(p, *(s32 *)(p + 0xc) - 5, 0x3c, 1, 0);
    }
    if (*(s32 *)(p + 0xc) >= 0x113 && func_ov001_022c69ac(p, *(s32 *)(p + 0xc) - 0x113, 0x1e, 1, 0)) {
        func_ov001_022c7a34(p, func_ov001_022c6ef4);
        return;
    }
    if (*(s32 *)(p + 0x698) == *(s32 *)(p + 0x69c)) {
        return;
    }
    gDisplayMain[4] = 0;
    gDisplayMain[5] = 0;
    Task_Create(func_ov001_022c6bd4, 0, 0, 0xf400, 0x1173, 0);
}
