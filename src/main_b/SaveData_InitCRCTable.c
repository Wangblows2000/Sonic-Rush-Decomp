#include <nitro/types.h>

extern u8 gSaveCRCTable;
void func_0201b0a4(void *p, u32 poly);
void SaveData_InitCRCTable(void) { func_0201b0a4(&gSaveCRCTable, 0xedb88320); }
