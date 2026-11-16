#include <nitro/types.h>

extern u32 gSystemFlags;
extern void *gWirelessMgrTask;
extern u8 gWirelessSendBuffer[];
extern u8 data_022501a0[];

BOOL func_02010b54(void);
void WH_SetReceiver(void (*fn)(void));
u32 WH_GetSystemState(void);
void WH_End(void);
void WFS_End(void);
void WH_Reset(void);
void Task_Destroy(void *p);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

static inline void Clear32_02042d34(void *dest, u32 size) {
    vu32 z = 0;
    MIi_CpuClear32(z, dest, size);
}

void WirelessMgr_StopWfs(s32 mode) {
    BOOL loop;
    u32 cnt;

    loop = TRUE;
    switch (mode) {
    case 0:
        cnt = 0;
        gSystemFlags &= ~0x100;
        WFS_End();
        if (!func_02010b54())
            WH_SetReceiver(NULL);
        do {
            switch (WH_GetSystemState()) {
            case 1:
                WH_End();
                break;
            case 0:
                loop = FALSE;
                break;
            case 3:
                break;
            case 9:
            case 10:
                if (++cnt > 3)
                    loop = FALSE;
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
        Clear32_02042d34(gWirelessSendBuffer, 0x200);
        Clear32_02042d34(data_022501a0, 0x240);
        gSystemFlags |= 0x200;
        gSystemFlags &= ~0x100;
        break;
    case 1:
        if (!func_02010b54())
            WH_SetReceiver(NULL);
        break;
    }
    if (gWirelessMgrTask == NULL)
        return;
    Task_Destroy(gWirelessMgrTask);
    gWirelessMgrTask = NULL;
}
