/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 OS_DisableInterrupts();
extern u8 data_0207d090[];
s32 func_02006590();
s32 func_02005bd4();
s32 OS_RestoreInterrupts();

void func_02006628(u8 *p0) {
    s32 t0 = OS_DisableInterrupts();
    s32 t1 = *(u32 *)((u8 *)data_0207d090 + 0x8);
    if (*(u32 *)((u8 *)p0 + 0x4) == t1) {
        *(u32 *)((u8 *)p0 + 0x8) = (*(u32 *)((u8 *)p0 + 0x8) - 0x1);
        if (*(u32 *)((u8 *)p0 + 0x8) == 0) {
            func_02006590(t1, p0);
            *(u32 *)((u8 *)p0 + 0x4) = 0;
            func_02005bd4(p0, 0);
        }
    }
    OS_RestoreInterrupts(t0);
}
