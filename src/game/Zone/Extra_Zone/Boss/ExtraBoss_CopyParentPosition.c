#include <nitro/types.h>

void ExtraBoss_CopyParentPosition(u8 *p) {
    u8 *q = *(u8 **)(p + 0x7c);
    *(s32 *)(p + 0x18) = *(s32 *)(q + 0x18);
    *(s32 *)(p + 0x1c) = *(s32 *)(q + 0x1c);
    *(s32 *)(p + 0x20) = *(s32 *)(q + 0x20);
}
