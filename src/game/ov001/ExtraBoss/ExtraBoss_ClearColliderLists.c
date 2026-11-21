#include <nitro/types.h>

extern s32 data_ov001_0231f334[3];
extern s32 data_ov001_0231f328[3];

void ExtraBoss_ClearColliderLists(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        data_ov001_0231f334[i] = 0;
        data_ov001_0231f328[i] = 0;
    }
}
