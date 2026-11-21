#include <nitro/types.h>

extern u16 gSaveState;
s32 OS_GetLockID(void);
void SaveData_InitCRCTable(void);
BOOL SaveData_Init(void) { s32 id = OS_GetLockID(); if (id == -3) return FALSE; gSaveState = id; SaveData_InitCRCTable(); return TRUE; }
