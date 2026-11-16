#include <nitro/types.h>

BOOL SaveData_Format(void);
void func_0205a304(void);
void func_0205a27c(s32 a, s32 b);
BOOL func_0205a33c(void) { if (!SaveData_Format()) return TRUE; func_0205a304(); func_0205a27c(2, 0); return FALSE; }
