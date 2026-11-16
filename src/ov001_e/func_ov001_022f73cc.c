#include <nitro/types.h>

extern u32 data_ov001_0231d578[10];
extern s32 data_ov001_0231d550[10];

s32 func_ov001_022f73cc(u32 v) {
    s32 i;
    s32 r;

    i = 0;
    r = 0;
    for (; i < 10; i++) {
        if (data_ov001_0231d578[i] >= v) {
            r = data_ov001_0231d550[i];
            break;
        }
    }
    return r;
}
