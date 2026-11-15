#include <nitro/types.h>

asm void func_0202d994(void)
{
    tst r1, #0x80000000
    bne _64
    ldr r2, [pc, #0x78]
    subs r2, r2, r1, lsr #20
    blt _7c
    cmp r2, #0x40
    bge _58
    mov r12, r1, lsl #0xb
    orr r12, r12, #0x80000000
    orr r12, r12, r0, lsr #21
    cmp r2, #0x20
    ble _40
    sub r2, r2, #0x20
    mov r1, #0
    mov r0, r12, lsr r2
    bx lr
_40:
    mov r3, r0, lsl #0xb
    mov r1, r12, lsr r2
    mov r0, r3, lsr r2
    rsb r2, r2, #0x20
    orr r0, r0, r12, lsl r2
    bx lr
_58:
    mov r1, #0
    mov r0, #0
    bx lr
_64:
    cmn r1, #0x100000
    cmpeq r0, #0
    bhi _7c
    mov r1, #0
    mov r0, #0
    bx lr
_7c:
    mvn r1, #0
    mvn r0, #0
    bx lr
_88:
    dcd 0x0000043e
}
