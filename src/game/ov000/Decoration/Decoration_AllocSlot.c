#include <nitro/types.h>

extern u32 gDecorationSlotMask;

s32 Decoration_AllocSlot(void) {
    u8 i;

    for (i = 0; i < 32; i++) {
        if (!(gDecorationSlotMask & (1 << i))) {
            gDecorationSlotMask |= 1 << i;
            return i;
        }
    }
    return i;
}
