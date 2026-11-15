#include <nitro/types.h>

void func_ov001_022d1404(void *p, void (*f)(void));
void func_ov001_022d0c84(void);

#pragma thumb on
void func_ov001_022d0d34(void *p) {
    func_ov001_022d1404(p, func_ov001_022d0c84);
}
#pragma thumb reset
