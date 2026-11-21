#include <nitro/types.h>

void MI_SetWramBank(u8 v);
void MI_StopDma(u32 v);
void MI_Init(void) { MI_SetWramBank(3); MI_StopDma(0); }
