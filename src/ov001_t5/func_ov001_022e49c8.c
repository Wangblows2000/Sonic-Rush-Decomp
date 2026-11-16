#include <nitro/types.h>

s32 func_ov001_022e2ad8(void *a, void *b);
void func_ov001_022e4748(void *a, void *b);
void func_ov001_022e4198(void *a, void *b);
void func_ov001_022e4494(void *a, void *b);
void func_ov001_022e4000(void *a, void *b);
void func_ov001_022e432c(void *a, void *b);

#pragma thumb on
void func_ov001_022e49c8(void *a, void *b) {
    switch (func_ov001_022e2ad8(a, b)) {
    case 0:
        func_ov001_022e4748(a, b);
        func_ov001_022e4198(a, b);
        break;
    case 1:
        func_ov001_022e4494(a, b);
        func_ov001_022e4000(a, b);
        break;
    }
    func_ov001_022e432c(a, b);
}
#pragma thumb reset
