#include <nitro/types.h>

void func_ov001_022e23b0(s32 a);
void func_ov001_022e22d0(s32 a);
void func_0204ed24(void);
void func_0205a4ec(void (*f)(void));

#pragma thumb on
void func_ov001_022e2480(s32 *p) {
    if (p[0] == 0) {
        func_ov001_022e23b0(p[1]);
    } else {
        func_ov001_022e22d0(p[2]);
    }
    func_0204ed24();
    func_0205a4ec(NULL);
}
#pragma thumb reset
