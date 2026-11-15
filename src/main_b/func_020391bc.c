#include <nitro/types.h>

extern u16 *data_02072c98[][4];
void func_020391bc(s32 a, s32 b, u16 *out1, u16 *out2) { *out1 = 0; if (a == 0) *out1 = (*(vu32 *)0x4000000 & 0x7000000) >> 24; *out2 = (*data_02072c98[a][b] & 0x3c) >> 2; }
