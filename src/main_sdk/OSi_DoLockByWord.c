#include <nitro/types.h>

s32 OSi_DoTryLockByWord(u32 a, void *b, void *c, u32 d);
void OSi_WaitByLoop(void);
void OSi_DoLockByWord(u32 a, void *b, void *c, u32 d) {
    if (OSi_DoTryLockByWord(a, b, c, d) <= 0) return;
    do { OSi_WaitByLoop(); } while (OSi_DoTryLockByWord(a, b, c, d) > 0);
}
