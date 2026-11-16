#include <nitro/types.h>

extern u8 gSaveCRCTable;
void func_0201b028(void *a, void *b, u32 n);
void SaveData_CalcChecksum(u8 *p) { func_0201b028(&gSaveCRCTable, p + 4, 0x310); }
