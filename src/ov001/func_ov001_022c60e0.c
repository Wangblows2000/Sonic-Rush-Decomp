#include <nitro/types.h>

s32 func_02057350(void *p);
void func_ov001_022c6160(void *p, void (*f)(void));
void func_ov001_022c6094(void);

void func_ov001_022c60e0(void *p) {
    if (func_02057350(p) == 1) {
        return;
    }
    func_ov001_022c6160(p, func_ov001_022c6094);
}
