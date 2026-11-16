// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gPlayers;

asm void MapObj74_IsPlayerInArea(void)
{
    stmdb sp!, {r4, r5, r6, lr}
    ldr r1, [r0, #0x1e0]
    ldr r3, [r0, #8]
    ldr r2, [pc, #0x80]
    ldrsb r12, [r1, #6]
    ldr r5, [r2, r3, lsl #2]
    ldr r3, [r0, #0x10]
    mov r2, #0
    ldr r4, [r5, #0x10]
    add lr, r3, r12, lsl #11
    mov r6, r2
    cmp lr, r4
    bgt _48
    ldrsb r3, [r1, #8]
    sub r3, r3, r12
    add r3, lr, r3, lsl #11
    cmp r3, r4
    movge r6, #1
_48:
    cmp r6, #0
    beq _88
    ldrsb r3, [r1, #7]
    ldr r0, [r0, #0x14]
    ldr lr, [r5, #0x14]
    add r12, r0, r3, lsl #11
    cmp r12, lr
    mov r4, #0
    bgt _80
    ldrsb r0, [r1, #9]
    sub r0, r0, r3
    add r0, r12, r0, lsl #11
    cmp r0, lr
    movge r4, #1
_80:
    cmp r4, #0
    movne r2, #1
_88:
    mov r0, r2
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    dcd gPlayers
}
