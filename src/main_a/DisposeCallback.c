#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
u32 func_0201d568(u32 v);
u32 func_0201d0f8(u32 idx);
void func_0201d0dc(u32 idx, u32 v);

void DisposeCallback(u32 a, u32 b, u32 idx) {
    u32 old, e;
    if (b == 0) return;
    e = OS_DisableInterrupts();
    old = func_0201d568(b);
    if (a == func_0201d0f8(idx)) {
        func_0201d0dc(idx, 0);
    }
    func_0201d568(old);
    OS_RestoreInterrupts(e);
}
