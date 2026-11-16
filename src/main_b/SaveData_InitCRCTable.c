#include <nitro/types.h>

extern u8 gSaveCRCTable;
void MATHi_CRC32InitTableRev(void *p, u32 poly);
void SaveData_InitCRCTable(void) { MATHi_CRC32InitTableRev(&gSaveCRCTable, 0xedb88320); }
