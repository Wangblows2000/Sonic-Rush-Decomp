#include <nitro/types.h>

#pragma thumb on
asm void func_020087f8(const void *src, u16 *dst)
{
    push {r4, r5, r6, r7}
    sub sp, #0xc
    mov r7, #0
    ldmia r0!, {r3}
    lsr r5, r3, #8
    mov r4, #0
_0c:
    cmp r5, #0
    ble _62
    ldrb r3, [r0]
    str r3, [sp, #4]
    add r0, #1
    ldr r3, [sp, #4]
    lsl r2, r3, #0x19
    lsr r2, r2, #0x19
    ldr r6, [sp, #4]
    lsr r3, r6, #8
    bcs _40
    add r2, #1
    sub r5, r5, r2
_26:
    ldrb r6, [r0]
    lsl r6, r4
    orr r7, r6
    add r0, #1
    mov r3, #8
    eor r4, r3
    bne _3a
    strh r7, [r1]
    add r1, #2
    mov r7, #0
_3a:
    sub r2, r2, #1
    bgt _26
    b _0c
_40:
    add r2, #3
    sub r5, r5, r2
    ldrb r6, [r0]
    str r6, [sp, #8]
    add r0, #1
_4a:
    ldr r6, [sp, #8]
    lsl r6, r4
    orr r7, r6
    mov r3, #8
    eor r4, r3
    bne _5c
    strh r7, [r1]
    add r1, #2
    mov r7, #0
_5c:
    sub r2, r2, #1
    bgt _4a
    b _0c
_62:
    add sp, #0xc
    pop {r4, r5, r6, r7}
    bx lr
}
#pragma thumb reset
