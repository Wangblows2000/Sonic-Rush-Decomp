#include <nitro/types.h>

asm void _ll_udiv(void)
{
    stmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, #0
    dcd 0xea000001  // b _ull_mod+0x8 (mid-function entry; no symbol, raw encoding)
}
