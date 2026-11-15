// cc: 1.2/sp2 (does not match default 1.2/sp3)
#include <nitro/types.h>

#pragma thumb on

extern u8 data_02072cb8;
extern u32 *data_0207ca10[];
extern s16 data_022c4da0;
u32 func_0206039c(void) { u32 idx; switch (data_02072cb8) { case 0: case 1: case 2: case 3: case 4: case 5: idx = data_02072cb8; break; default: idx = 1; break; } return data_0207ca10[idx][data_022c4da0]; }

#pragma thumb reset
