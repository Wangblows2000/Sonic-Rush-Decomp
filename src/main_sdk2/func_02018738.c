#include <nitro/types.h>
#include <main_sdk2/wh.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
BOOL func_0201a4a4(void);
void func_0201a294(void (*cb)(void));
BOOL func_020187cc(void);
void func_020187c0(void);

BOOL func_02018738(void)
{
    BOOL ret = TRUE;
    u32 e = OS_DisableInterrupts();
    if (data_02080f18->unk526 == 0) {
        data_02080f18->unk5e4 = 0;
        data_02080f18->unk526 = 1;
        if (func_0201a4a4()) {
            func_0201a294(func_020187c0);
            ret = FALSE;
        } else {
            ret = func_020187cc();
        }
    }
    OS_RestoreInterrupts(e);
    return ret;
}
