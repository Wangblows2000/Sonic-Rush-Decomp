#include <nitro/types.h>

void OSi_DoUnlockByWord(u32 a, u32 b, u32 c, u32 d);
void OS_UnlockByWord(u32 a, u32 b, u32 c) { OSi_DoUnlockByWord(a, b, c, 0); }
