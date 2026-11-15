#include <nitro/types.h>

typedef struct { u8 pad[0x10]; s32 *work; } UnkStruct_022c8994;
UnkStruct_022c8994 *func_020372d0(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov001_022c8958(void);

#pragma thumb on
void func_ov001_022c8994(void) {
    s32 *w = func_020372d0(func_ov001_022c8958, 0, 0, 0x4000, 0x1000, 8)->work;
    w[0] = 0;
    w[1] = 0;
}
#pragma thumb reset
