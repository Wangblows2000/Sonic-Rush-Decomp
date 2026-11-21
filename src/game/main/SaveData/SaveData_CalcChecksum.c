#include <nitro/types.h>

extern u8 gSaveCRCTable;
void MATH_CalcCRC32(void *a, void *b, u32 n);
void SaveData_CalcChecksum(u8 *p) { MATH_CalcCRC32(&gSaveCRCTable, p + 4, 0x310); }
