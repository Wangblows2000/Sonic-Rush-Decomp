#include <nitro/types.h>

void func_ov001_022d1404(void *p, void (*f)(void));
void func_ov001_022d1350(void);

#pragma thumb on
void func_ov001_022d13f4(void *p) {
    func_ov001_022d1404(p, func_ov001_022d1350);
}
#pragma thumb reset
