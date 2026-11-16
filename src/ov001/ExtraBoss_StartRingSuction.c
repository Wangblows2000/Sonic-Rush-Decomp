#include <nitro/types.h>

extern u8 *gExtraBossRingWork;
void ExtraBoss_UpdateRingsSuction(void);

void ExtraBoss_StartRingSuction(void) {
    *(void (**)(void))(gExtraBossRingWork + 0x90) = ExtraBoss_UpdateRingsSuction;
}
