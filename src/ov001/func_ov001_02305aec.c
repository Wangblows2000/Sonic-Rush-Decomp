#include <nitro/types.h>

extern u8 *data_ov001_0231f5a8;
void *func_020372d0(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
u8 *func_ov001_022ffce8(void);
void func_020366f4(s32 a, s32 b, s32 c);
void func_ov001_023059b4(void);

void func_ov001_02305aec(void) {
    s32 i;
    *(void **)(data_ov001_0231f5a8 + 0x14) = func_020372d0(func_ov001_023059b4, 0, 0, 0x3100, 2, 0);
    *(u16 *)(data_ov001_0231f5a8 + 0x5e6) = 0;
    for (i = 4; i <= 7; i++) {
        *(s16 *)(data_ov001_0231f5a8 + i * 0x48 + 0x9c) = 0x80;
        *(s16 *)(data_ov001_0231f5a8 + i * 0x48 + 0x9e) = 0x5b;
    }
    if (*(s32 *)(func_ov001_022ffce8() + 0x3c) == 0) {
        func_020366f4(0, 0, 0x22);
    } else {
        func_020366f4(0, 0, 0x2b);
    }
}
