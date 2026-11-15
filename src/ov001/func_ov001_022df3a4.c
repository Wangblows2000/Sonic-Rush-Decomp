#include <nitro/types.h>

void func_ov001_022de704(void *a, s32 *p);
void func_ov001_022dedd4(void *a, s32 *p);

void func_ov001_022df3a4(void *a, s32 *p) {
    if (*p != 2) {
        func_ov001_022dedd4(a, p);
    } else {
        func_ov001_022de704(a, p);
    }
}
