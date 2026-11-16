#include <nitro/types.h>

extern u8 *data_ov001_0231f094;

void ExtraBoss_CancelSwordFx(void) {
    data_ov001_0231f094[1] = 1;
}
