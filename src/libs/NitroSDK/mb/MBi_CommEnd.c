#include <nitro/types.h>
#include <main_sdk2/wh.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
BOOL MBi_IsTaskAvailable(void);
void MBi_EndTaskThread(void (*cb)(void));
BOOL MBi_CallReset(void);
void MBi_OnReset(void);

BOOL MBi_CommEnd(void)
{
    BOOL ret = TRUE;
    u32 e = OS_DisableInterrupts();
    if (data_02080f18->unk526 == 0) {
        data_02080f18->unk5e4 = 0;
        data_02080f18->unk526 = 1;
        if (MBi_IsTaskAvailable()) {
            MBi_EndTaskThread(MBi_OnReset);
            ret = FALSE;
        } else {
            ret = MBi_CallReset();
        }
    }
    OS_RestoreInterrupts(e);
    return ret;
}
