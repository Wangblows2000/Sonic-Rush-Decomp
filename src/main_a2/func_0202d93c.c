#include <nitro/types.h>

asm void func_0202d93c(void)
{
    tst r1, #0x80000000
    bne _38
    ldr r2, [pc, #0x44]
    subs r2, r2, r1, lsr #20
    blt _4c
    cmp r2, #0x20
    bge _30
    mov r3, r1, lsl #0xb
    orr r3, r3, #0x80000000
    orr r3, r3, r0, lsr #21
    mov r0, r3, lsr r2
    bx lr
_30:
    mov r0, #0
    bx lr
_38:
    cmn r1, #0x100000
    cmpeq r0, #0
    movls r0, #0
    mvnhi r0, #0
    bx lr
_4c:
    mvn r0, #0
    bx lr
_54:
    dcd 0x0000041e
}
