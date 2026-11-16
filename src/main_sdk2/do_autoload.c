#include <nitro/types.h>

void AutoloadCallback(void);

asm void do_autoload(void)
{
    ldr r0, =0x2000b00
    ldr r1, [r0]
    ldr r2, [r0, #4]
    ldr r3, [r0, #8]
@1:
    cmp r1, r2
    beq @4
    ldr r4, [r1], #4
    ldr r5, [r1], #4
    add r6, r4, r5
@2:
    cmp r4, r6
    ldrmi r7, [r3], #4
    strmi r7, [r4], #4
    bmi @2
    ldr r5, [r1], #4
    add r6, r4, r5
    mov r7, #0
@3:
    cmp r4, r6
    strlo r7, [r4], #4
    blo @3
    beq @1
@4:
    b AutoloadCallback
}

