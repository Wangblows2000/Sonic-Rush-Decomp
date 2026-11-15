#include <nitro/types.h>

void func_ov001_022debf0(void *a, s32 *p);
void func_ov001_022df0b4(void *a, s32 *p);

void func_ov001_022df440(void *a, s32 *p) {
    if (*p != 2) {
        func_ov001_022df0b4(a, p);
    } else {
        func_ov001_022debf0(a, p);
    }
}
