// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

asm void func_02026f88(void)
{
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    ldrh r4, [r3, #4]
    ldr lr, [r2, #4]
    mov r12, r1, asr #0xc
    sub r4, r4, #1
    cmp r12, r4
    add r4, r3, lr
    ldr lr, [r2]
    bne _b0
    ands r2, lr, #0xc0000000
    beq _44
    ands r2, lr, #0x40000000
    andne r2, r12, #1
    addne r12, r2, r12, lsr #1
    andeq r2, r12, #3
    addeq r12, r2, r12, lsr #2
_44:
    ldr r2, [r3, #8]
    ands r2, r2, #2
    beq _8c
    ldr r2, [pc, #0x114]
    ands r3, lr, #0x20000000
    and r3, r1, r2
    movne r1, r12, lsl #1
    ldrnesh r2, [r4, r1]
    ldrnesh r1, [r4]
    add sp, sp, #4
    ldreq r2, [r4, r12, lsl #2]
    ldreq r1, [r4]
    sub r1, r1, r2
    mul r1, r3, r1
    add r1, r2, r1, asr #12
    str r1, [r0]
    ldmia sp!, {r4, r5, lr}
    bx lr
_8c:
    ands r1, lr, #0x20000000
    movne r1, r12, lsl #1
    ldrnesh r1, [r4, r1]
    add sp, sp, #4
    strne r1, [r0]
    ldreq r1, [r4, r12, lsl #2]
    streq r1, [r0]
    ldmia sp!, {r4, r5, lr}
    bx lr
_b0:
    ands r2, lr, #0xc0000000
    beq _118
    ldr r2, [pc, #0xb0]
    ands r3, lr, #0x40000000
    and r2, lr, r2
    mov r2, r2, lsr #0x10
    beq _f0
    cmp r12, r2
    movhs r12, r2, lsr #1
    bhs _118
    ldr r2, [pc, #0x94]
    mov r12, r12, lsr #1
    and r5, r1, r2
    mov r2, #2
    mov r1, #1
    b _128
_f0:
    cmp r12, r2
    andhs r2, r12, #3
    addhs r12, r2, r12, lsr #2
    bhs _118
    ldr r2, [pc, #0x70]
    mov r12, r12, lsr #2
    and r5, r1, r2
    mov r2, #4
    mov r1, #2
    b _128
_118:
    ldr r3, [pc, #0x4c]
    mov r2, #1
    and r5, r1, r3
    mov r1, #0
_128:
    ands r3, lr, #0x20000000
    addne r3, r4, r12, lsl #1
    movne lr, r12, lsl #1
    ldrnesh r12, [r4, lr]
    ldrnesh r3, [r3, #2]
    addeq r3, r4, r12, lsl #2
    ldreq r12, [r4, r12, lsl #2]
    ldreq r3, [r3, #4]
    mul r4, r12, r2
    sub r3, r3, r12
    mul r2, r5, r3
    add r2, r4, r2, asr #12
    mov r1, r2, asr r1
    str r1, [r0]
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
    dcd 0x00000fff
    dcd 0x1fff0000
    dcd 0x00001fff
    dcd 0x00003fff
}
