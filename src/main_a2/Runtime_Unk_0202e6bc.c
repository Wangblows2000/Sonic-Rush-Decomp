#include <nitro/types.h>

asm void Runtime_Unk_0202e6bc(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    movcc r0, #0
    bxcc lr
    cmp r3, r1, lsl #1
    movcc r0, #0
    bxcc lr
    mov r0, #1
    bx lr
}
