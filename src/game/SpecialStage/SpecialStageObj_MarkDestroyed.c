#include <nitro/types.h>

void SpecialStageObj_MarkDestroyed(u8 *p) {
    *(u16 *)(p + 0x60) |= 1;
}
