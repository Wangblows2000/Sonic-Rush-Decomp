#include <nitro/types.h>

extern s32 data_ov001_0231f550[];

void ExtraBoss_ReleaseBgFiles(void) {
    s32 i;
    for (i = 0; i < 0x16; i++) {
        data_ov001_0231f550[i] = 0;
    }
}
