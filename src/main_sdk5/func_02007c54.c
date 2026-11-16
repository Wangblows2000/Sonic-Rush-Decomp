#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void func_02007b3c(u32 dmaNo);
void func_02005448(u32 dmaNo, void (*callback)(void *), void *arg);
void func_01ff8520(u32 dmaNo, u32 src, u32 dest, u32 ctrl);

void func_02007c54(u32 dmaNo, void *dest, u32 data, u32 size, void (*callback)(void *), void *arg)
{
    if (size == 0) {
        if (callback) {
            callback(arg);
        }
    } else {
        func_02007b3c(dmaNo);
        if (callback) {
            func_02005448(dmaNo, callback, arg);
            {
                u32 enabled = OS_DisableInterrupts();
                vu32 *src = (vu32 *)0x040000e0 + dmaNo;
                *src = data;
                func_01ff8520(dmaNo, (u32)src, (u32)dest, 0xc5000000 | (size >> 2));
                (void)OS_RestoreInterrupts(enabled);
            }
        } else {
            {
                u32 enabled = OS_DisableInterrupts();
                vu32 *src = (vu32 *)0x040000e0 + dmaNo;
                *src = data;
                func_01ff8520(dmaNo, (u32)src, (u32)dest, 0x85000000 | (size >> 2));
                (void)OS_RestoreInterrupts(enabled);
            }
        }
    }
}
