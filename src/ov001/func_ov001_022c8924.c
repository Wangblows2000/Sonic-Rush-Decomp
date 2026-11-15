#include <nitro/types.h>

void func_02059ba0(s32 a);

#pragma thumb on
void func_ov001_022c8924(u8 *p, s32 v) {
    if (*(s32 *)(p + 8) != v) {
        *(s32 *)(p + 8) = v;
        switch (v) {
        case 0:
            func_02059ba0(1);
            break;
        case 1:
            func_02059ba0(0);
            break;
        }
    }
}
#pragma thumb reset
