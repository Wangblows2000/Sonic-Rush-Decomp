#include <nitro/types.h>

asm void _ull_mod(void)
{
    stmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, #1
    orrs r5, r3, r2
    bne _18
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
_18:
    orrs r5, r1, r3
    dcd 0x1a000027  // bne func_0202f708+0x74 (mid-function entry; no symbol, raw encoding)
    mov r1, r2
    dcd 0xeb000101  // bl _u32_div_f+0x8 (mid-function entry; no symbol, raw encoding)
    cmp r4, #0
    movne r0, r1
    mov r1, #0
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
    stmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, r1
    orr r4, r4, #1
    dcd 0xea000003  // b func_0202f708+0x10 (mid-function entry; no symbol, raw encoding)
}
