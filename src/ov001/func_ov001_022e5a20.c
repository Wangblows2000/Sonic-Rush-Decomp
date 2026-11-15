#include <nitro/types.h>

void func_ov001_022e5928(void *p);
void func_ov001_022e57d4(void *p);

#pragma thumb on
void func_ov001_022e5a20(s32 *p) {
    switch (*p) {
    case 0:
        func_ov001_022e5928(p);
        break;
    case 1:
        func_ov001_022e57d4(p);
        break;
    }
}
#pragma thumb reset
