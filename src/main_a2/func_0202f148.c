#include <nitro/types.h>

asm void func_0202f148(void)
{
    bic r1, r0, #0x80000000
    mov r2, #0x9e
    subs r2, r2, r1, lsr #23
    ble _28
    mov r1, r1, lsl #8
    orr r1, r1, #0x80000000
    cmp r0, #0
    mov r0, r1, lsr r2
    rsbmi r0, r0, #0
    bx lr
_28:
    mvn r0, r0, asr #31
    add r0, r0, #0x80000000
    bx lr
}
