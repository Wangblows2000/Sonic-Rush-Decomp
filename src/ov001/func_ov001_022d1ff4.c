#include <nitro/types.h>

s32 func_ov001_022d0158(s32 a);
void func_ov001_022d2234(void *p, void (*f)(void));
void func_ov001_022d1fd4(void);

#pragma thumb on
void func_ov001_022d1ff4(u8 *p) {
    *(s32 *)(p + 0xbd0) = func_ov001_022d0158(2);
    func_ov001_022d2234(p, func_ov001_022d1fd4);
}
#pragma thumb reset
