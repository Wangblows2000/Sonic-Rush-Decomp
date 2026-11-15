#include <nitro/types.h>

extern u8 data_02250464[];
void func_ov001_022c6ac4(void *p, s32 a, s32 b, s32 c, s32 d);
BOOL func_ov001_022c69ac(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov001_022c7a34(void *p, void (*f)(void));
void func_0201c070(void *p, s32 a);
void func_ov001_022c73e8(void);

void func_ov001_022c7430(u8 *p) {
    if (*(s32 *)(p + 0xc) >= 5) {
        func_ov001_022c6ac4(p, *(s32 *)(p + 0xc) - 5, 0x3c, 1, 0);
    }
    if (*(s32 *)(p + 0xc) >= 0x3c5) {
        func_ov001_022c69ac(p, *(s32 *)(p + 0xc) - 0x3c5, 0xb4, 1, 1);
    }
    if (*(s32 *)(p + 0xc) == 0x3c5) {
        func_0201c070(data_02250464, 0xb4);
    }
    if (*(s32 *)(p + 0xc) < 0x483) {
        return;
    }
    func_ov001_022c7a34(p, func_ov001_022c73e8);
}
