/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 OS_DisableInterrupts();
s32 func_02005bd4();
s32 func_02005ba0();
s32 OS_RestoreInterrupts();

void func_0200fcb8(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x28);
    s32 t1 = *(u32 *)((u8 *)p0 + 0x2c);
    s32 t2 = OS_DisableInterrupts();
    s32 t3 = *(u32 *)((u8 *)p0 + 0x34);
    *(u32 *)((u8 *)p0 + 0x34) = (*(u32 *)((u8 *)p0 + 0x34) & ~0xc);
    func_02005bd4(((u8 *)p0 + 0xd4), t3);
    if ((*(u32 *)((u8 *)p0 + 0x34) & 0x10) != 0) {
        func_02005ba0(((u8 *)p0 + 0x3c));
    }
    OS_RestoreInterrupts(t2);
    if (t0 == 0) {
        return;
    }
    ((s32 (*)())t0)(t1);
}
