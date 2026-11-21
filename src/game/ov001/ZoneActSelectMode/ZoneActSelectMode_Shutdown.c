#include <nitro/types.h>

void NaviMsg_Destroy(void);
void NaviM_Destroy(void);
void NaviM_Release(void);
void NaviMsg_CloseFiles(void);
void SysSound_Release(void);
void Touch_Disable(void);
void ZoneActSelectMode_ResetDisplay(void);

#pragma thumb on
void ZoneActSelectMode_Shutdown(void) {
    NaviMsg_Destroy();
    NaviM_Destroy();
    NaviM_Release();
    NaviMsg_CloseFiles();
    SysSound_Release();
    Touch_Disable();
    ZoneActSelectMode_ResetDisplay();
}
#pragma thumb reset
