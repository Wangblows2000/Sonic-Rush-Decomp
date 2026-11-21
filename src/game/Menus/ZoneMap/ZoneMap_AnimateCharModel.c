#include <nitro/types.h>

void Model_Animate(void *p);

#pragma thumb on
void ZoneMap_AnimateCharModel(u8 *p) {
    Model_Animate(p + 0x24);
}
#pragma thumb reset
