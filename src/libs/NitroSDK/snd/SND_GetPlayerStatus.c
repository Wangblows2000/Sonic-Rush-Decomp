#include <nitro/types.h>

void DC_InvalidateRange(void *p, u32 size);
extern u32 *data_0207ef20;
u32 SND_GetPlayerStatus(void) { DC_InvalidateRange(data_0207ef20, 0x280); return data_0207ef20[1]; }
