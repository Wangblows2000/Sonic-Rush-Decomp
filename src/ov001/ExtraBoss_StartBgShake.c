#include <nitro/types.h>

extern u8 *data_ov001_0231f54c;

void ExtraBoss_StartBgShake(s32 a, s32 b) {
    *(s32 *)(data_ov001_0231f54c + 0xb4c) = a;
    *(s32 *)(data_ov001_0231f54c + 0xb50) = b * 2 + 1;
}
