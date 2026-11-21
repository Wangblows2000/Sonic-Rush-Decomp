#include <nitro/types.h>

void MBi_StartParentCore(u32 a);
extern u8 *data_02080f1c;
void MB_StartParent(u32 a) { *(u32 *)(data_02080f1c + 0x1320) = 0; MBi_StartParentCore(a); }
