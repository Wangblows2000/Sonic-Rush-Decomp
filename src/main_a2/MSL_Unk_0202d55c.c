#include <nitro/types.h>

asm void MSL_Unk_0202d55c(void)
{
    stmfd sp!, {r0, r1, r2, r3}
    ldr r0, [sp]
    cmp r0, #0
    beq _70
    add r2, sp, #0
    ldrb r1, [r2]
    mov r0, #0
    cmp r1, #0
    bne _34
_24:
    ldrb r1, [r2, #1]!
    add r0, r0, #8
    cmp r1, #0
    beq _24
_34:
    mvn r1, r1
    strb r1, [r2]
    ldrb r1, [r2]
    ands r1, r1, #1
    addeq sp, sp, #0x10
    bxeq lr
_4c:
    ldrb r1, [r2]
    add r0, r0, #1
    mov r1, r1, asr #1
    strb r1, [r2]
    ldrb r1, [r2]
    ands r1, r1, #1
    bne _4c
    add sp, sp, #0x10
    bx lr
_70:
    mov r0, #0x20
    add sp, sp, #0x10
    bx lr
}
