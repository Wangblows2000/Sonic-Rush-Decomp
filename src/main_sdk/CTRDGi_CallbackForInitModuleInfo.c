#include <nitro/types.h>

extern u16 data_02080f54;
void OS_Terminate(void);
void CTRDGi_CallbackForInitModuleInfo(u32 a, u32 b) { if ((b & 0x3f) == 1) { data_02080f54 = 1; return; } OS_Terminate(); }
