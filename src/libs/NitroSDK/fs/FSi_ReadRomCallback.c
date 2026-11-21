#include <nitro/types.h>

void FSi_OnRomReadDone(u32 a);
extern u32 data_0207efc0;
void CARDi_ReadRom(u32 a, u32 b, u32 c, u32 d, void *e, u32 f, u32 g);
u32 FSi_ReadRomCallback(u32 a, u32 b, u32 c, u32 d) { CARDi_ReadRom(data_0207efc0, c, b, d, FSi_OnRomReadDone, a, 1); return 6; }
