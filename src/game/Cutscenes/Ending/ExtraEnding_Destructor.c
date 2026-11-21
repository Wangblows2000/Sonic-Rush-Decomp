#include <nitro/types.h>

extern u8 *gTaskCurrent;
void ExtraEnding_ReleaseSound(void *p);

void ExtraEnding_Destructor(void) {
    ExtraEnding_ReleaseSound(*(void **)(gTaskCurrent + 0x10));
}
