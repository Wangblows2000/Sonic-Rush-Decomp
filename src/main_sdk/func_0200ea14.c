#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
extern u32 data_0207f09c;
BOOL func_0200ea14(void) {
    u32 e = OS_DisableInterrupts();
    if (data_0207f09c != 0) { OS_RestoreInterrupts(e); return 0; }
    data_0207f09c = 1;
    OS_RestoreInterrupts(e);
    return 1;
}
