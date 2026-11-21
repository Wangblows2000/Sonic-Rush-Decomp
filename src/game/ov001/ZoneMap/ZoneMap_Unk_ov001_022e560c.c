#include <nitro/types.h>

#pragma thumb on
void ZoneMap_Unk_ov001_022e560c(u8 *p) {
    *(s32 *)(p + 0x60) += *(s32 *)(p + 0x5c);
}
#pragma thumb reset
