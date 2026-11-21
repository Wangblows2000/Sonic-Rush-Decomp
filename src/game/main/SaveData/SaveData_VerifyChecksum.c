#include <nitro/types.h>

u32 SaveData_CalcChecksum(u32 *p);
BOOL SaveData_VerifyChecksum(u32 *p) { return *p == SaveData_CalcChecksum(p); }
