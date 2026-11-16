#include <nitro/types.h>

#pragma thumb on

extern u8 gGameLanguage;
extern u32 *data_0207ca10[];
extern s16 gTalkDemoId;
u32 func_0206039c(void) { u32 idx; switch (gGameLanguage) { case 0: case 1: case 2: case 3: case 4: case 5: idx = gGameLanguage; break; default: idx = 1; break; } return data_0207ca10[idx][gTalkDemoId]; }

#pragma thumb reset
