#include <nitro/types.h>

void func_ov001_022deb20(void *a, s32 *p);
void func_ov001_022df008(void *a, s32 *p);

void func_ov001_022df40c(void *a, s32 *p) {
    if (*p != 2) {
        func_ov001_022df008(a, p);
    } else {
        func_ov001_022deb20(a, p);
    }
}
