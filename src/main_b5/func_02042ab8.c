#include <nitro/types.h>

extern void *data_022c4364;

u32 WH_GetSystemState(void);
void WH_End(void);
void WH_Reset(void);
s32 func_0203b08c(void);
void func_0203b52c(void);
void Task_Destroy(void *p);

void func_02042ab8(void) {
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
            switch (func_0203b08c()) {
            case 0:
            case 5:
                loop = FALSE;
                break;
            case 6:
                break;
            default:
                func_0203b52c();
                break;
            }
        } while (loop);
    }
    if (data_022c4364 == NULL)
        return;
    Task_Destroy(data_022c4364);
    data_022c4364 = NULL;
}
