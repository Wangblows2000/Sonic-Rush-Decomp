#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
BOOL func_0201a4a4(void);
extern u8 *data_02080f40;
void func_0201a2e8(void *p, u32 a, u32 b, u32 c);
void func_0201a294(u32 v) {
    u32 e = OS_DisableInterrupts();
    if (func_0201a4a4()) func_0201a2e8(data_02080f40 + 0x98, 0, v, 0);
    OS_RestoreInterrupts(e);
}
