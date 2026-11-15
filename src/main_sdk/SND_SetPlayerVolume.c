#include <nitro/types.h>

void func_020089c8(u32 a, u32 off, u32 v, u32 sz);
void SND_SetPlayerVolume(u32 a, u32 v) { func_020089c8(a, 6, v, 2); }
