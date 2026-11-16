// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void func_02068a34(void);

asm void func_ov000_023045c0(void)
{
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r4, r0
    ldr r0, [r4, #0x1e0]
    mov r2, #0x100
    ldrh r1, [r0, #4]
    mov r3, #0
    ands r0, r1, #8
    movne r0, r2, lsl #0x11
    movne r2, r0, asr #0x10
    ands r0, r1, #4
    beq _58
    ldr r1, [r4, #8]
    ldr r0, [pc, #0xc0]
    add r1, r1, r2
    str r1, [r4, #8]
    ldr r1, [r4, #8]
    add sp, sp, #4
    and r0, r1, r0
    str r0, [r4, #8]
    ldmia sp!, {r4, r5, lr}
    bx lr
_58:
    and r0, r1, #3
    add r0, r3, r0, lsl #14
    ldr r1, [r4, #0x1f4]
    mov r0, r0, lsl #0x10
    mov r3, r0, lsr #0x10
    ands r0, r1, #2
    beq _b8
    ldr r0, [r4, #8]
    add r5, r3, #0x2000
    mov r0, r0, lsl #0x10
    mov r1, r5, lsl #0x10
    mov r0, r0, lsr #0x10
    mov r1, r1, lsr #0x10
    bl func_02068a34
    str r0, [r4, #8]
    ldr r1, [r4, #8]
    mov r0, r5, lsl #0x10
    cmp r1, r0, lsr #16
    ldreq r0, [r4, #0x1f4]
    add sp, sp, #4
    eoreq r0, r0, #2
    streq r0, [r4, #0x1f4]
    ldmia sp!, {r4, r5, lr}
    bx lr
_b8:
    ldr r0, [r4, #8]
    sub r5, r3, #0x2000
    mov r0, r0, lsl #0x10
    mov r1, r5, lsl #0x10
    mov r0, r0, lsr #0x10
    mov r1, r1, lsr #0x10
    bl func_02068a34
    str r0, [r4, #8]
    ldr r1, [r4, #8]
    mov r0, r5, lsl #0x10
    cmp r1, r0, lsr #16
    ldreq r0, [r4, #0x1f4]
    eoreq r0, r0, #2
    streq r0, [r4, #0x1f4]
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
    dcd 0x0000ffff
}
