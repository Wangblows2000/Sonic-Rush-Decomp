#include <nitro/types.h>

void OSi_DoLockByWord(u32 a, u32 b, u32 c, u32 d);
void OS_LockByWord(u32 a, u32 b, u32 c) { OSi_DoLockByWord(a, b, c, 0); }
