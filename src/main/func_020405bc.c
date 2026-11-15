#include <nitro/types.h>

extern s32 data_022c1980;
extern s32 data_022c1984;

void func_020405bc(s32 value) {
    if ((u32)(data_022c1980 - 9) > 1) {
        data_022c1984 = value;
    }
}
