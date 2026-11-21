#include <nitro/types.h>

extern u32 gDecorationSlots[];
extern u32 gDecorationSlotMask;

void Decoration_FreeSlot(u32 *e) {
    u32 *p = gDecorationSlots;
    u8 i;

    for (i = 0; i < 32; i++) {
        if (p == e) {
            gDecorationSlotMask &= ~(1 << i);
            return;
        }
        p++;
    }
}
