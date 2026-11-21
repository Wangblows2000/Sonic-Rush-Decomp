#include <nitro/types.h>

extern u32 gSystemFlags;

void ZoneBg_DisableLineScroll(void) {
    gSystemFlags &= ~0xc00;
}
