#include <nitro/types.h>

BOOL SaveData_Format(void);
void SaveMsg_StopSystems(void);
void SaveMsg_Request(s32 a, s32 b);
BOOL SaveData_FormatOrShowError(void) { if (!SaveData_Format()) return TRUE; SaveMsg_StopSystems(); SaveMsg_Request(2, 0); return FALSE; }
