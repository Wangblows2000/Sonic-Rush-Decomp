#include <nitro/types.h>

void ZoneMap_AnimateScrollArrows(void *p);
void ZoneMap_DrawScrollArrows(void *p);

#pragma thumb on
void ZoneMap_UpdateScrollArrows(u8 *p) {
    u8 *q = *(u8 **)(p + 0xa4) + 0x1b08;
    ZoneMap_AnimateScrollArrows(q);
    ZoneMap_DrawScrollArrows(q);
}
#pragma thumb reset
