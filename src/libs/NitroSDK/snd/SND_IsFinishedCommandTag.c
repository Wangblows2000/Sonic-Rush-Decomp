#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
extern u32 data_0207d3f8;
BOOL SND_IsFinishedCommandTag(u32 id) {
    u32 e = OS_DisableInterrupts();
    u32 cur = data_0207d3f8;
    BOOL r;
    if (id > cur) { if (id - cur < 0x80000000) r = FALSE; else r = TRUE; }
    else { if (cur - id < 0x80000000) r = TRUE; else r = FALSE; }
    OS_RestoreInterrupts(e);
    return r;
}
