#include <nitro/types.h>

void NaviMsg_Destroy(void);
void NaviM_Destroy(void);
void NaviM_Release(void);
void NaviMsg_CloseFiles(void);
void Task_DestroyRange(s32 a, s32 b);

#pragma thumb on
void ZoneMap_Shutdown(void) {
    NaviMsg_Destroy();
    NaviM_Destroy();
    NaviM_Release();
    NaviMsg_CloseFiles();
    Task_DestroyRange(0x3000, 0x3000);
}
#pragma thumb reset
