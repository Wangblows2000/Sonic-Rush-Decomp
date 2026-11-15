#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
u32 func_02010b54(void);
u8 *func_02010b70(void);
u32 func_0201154c(u32 v) {
    u32 e = OS_DisableInterrupts();
    u32 r = func_02010b54();
    if (r != 0) { OS_RestoreInterrupts(e); return r; }
    *(u32 *)(func_02010b70() + 0xbc) = v;
    OS_RestoreInterrupts(e);
    return 0;
}
