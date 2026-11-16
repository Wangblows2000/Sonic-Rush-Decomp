/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 OS_DisableInterrupts();
s32 OS_RestoreInterrupts();
s32 func_02007464();

void func_02007318(u8 *p0) {
    s32 t0 = OS_DisableInterrupts();
    *(u32 *)((u8 *)p0 + 0x24) = 1;
    s32 t1 = *(u32 *)p0;
    if (t1 == 0) {
        OS_RestoreInterrupts(t0, t1);
        return;
    }
    func_02007464(p0);
    *(u32 *)p0 = 0;
    OS_RestoreInterrupts(t0, 0);
}
