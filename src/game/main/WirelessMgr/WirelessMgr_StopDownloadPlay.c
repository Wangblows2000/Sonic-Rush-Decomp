#include <nitro/types.h>

extern void *gWirelessMgrTask;

u32 WH_GetSystemState(void);
void WH_End(void);
void WH_Reset(void);
s32 MBP_GetState(void);
void MBP_Cancel(void);
void Task_Destroy(void *p);

void WirelessMgr_StopDownloadPlay(void) {
    BOOL loop;
    u32 cnt;
    BOOL f1;
    BOOL f2;

    loop = TRUE;
    if (WH_GetSystemState() != 0) {
        cnt = 0;
        f1 = FALSE;
        f2 = FALSE;
        do {
            switch (WH_GetSystemState()) {
            case 1:
                WH_End();
                break;
            case 0:
                loop = f2;
                break;
            case 3:
                break;
            case 9:
            case 10:
                if (++cnt > 3)
                    loop = f1;
                else
                    WH_Reset();
                break;
            case 2:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            default:
                WH_Reset();
                break;
            }
        } while (loop);
    } else {
        do {
            switch (MBP_GetState()) {
            case 0:
            case 5:
                loop = FALSE;
                break;
            case 6:
                break;
            default:
                MBP_Cancel();
                break;
            }
        } while (loop);
    }
    if (gWirelessMgrTask == NULL)
        return;
    Task_Destroy(gWirelessMgrTask);
    gWirelessMgrTask = NULL;
}
