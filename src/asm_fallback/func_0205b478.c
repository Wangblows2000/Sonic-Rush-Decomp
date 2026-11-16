// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gCamera;
extern u32 data_ov000_0236ab3c;
extern u32 gPlayers;

asm void func_0205b478(void)
{
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, [r0, #4]
    mov r12, #0
    add r2, r1, #0x400
    ldrh lr, [r2, #0x20]
    add r3, r1, #0x300
    orr lr, lr, #0x10
    strh lr, [r2, #0x20]
    strh r12, [r3, #0xac]
    ldrsh r3, [r3, #0x58]
    ldrsh r2, [r1, #0x34]
    cmp r3, r2
    strgth r3, [r1, #0x34]
    ldr r2, [r0, #0x488]
    ldr r12, [r1, #0x10]
    ldr r3, [r0, #0x480]
    ands r2, r2, #0x20000000
    ldreq r2, [pc, #0x88]
    sub r3, r12, r3
    str r12, [r1, #0x1c]
    andeq r3, r3, r2
    ldr r2, [r0, #0x480]
    ldr r0, [pc, #0x78]
    add r2, r2, r3
    str r2, [r1, #0x10]
    ldr r2, [r0, #0xc8]
    ands r2, r2, #1
    beq _94
    ldr r2, [r1, #0x10]
    ldr r1, [r0, #0x3c]
    add sp, sp, #4
    sub r1, r2, r1
    str r1, [r0, #0x68]
    str r1, [r0, #4]
    ldmia sp!, {lr}
    bx lr
_94:
    ldrsb r2, [r0, #0x3a]
    ldr r12, [pc, #0x40]
    ldr r1, [r0, #0x3c]
    ldr r3, [r12, r2, lsl #2]
    ldr r2, [pc, #0x38]
    ldr r3, [r3, #0x10]
    sub r1, r3, r1
    str r1, [r0, #4]
    ldrsb r1, [r2, #0x3a]
    ldr r0, [r2, #0x3c]
    ldr r1, [r12, r1, lsl #2]
    ldr r1, [r1, #0x10]
    sub r0, r1, r0
    str r0, [r2, #4]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
    dcd 0x0000ffff
    dcd gCamera
    dcd gPlayers
    dcd data_ov000_0236ab3c
}
