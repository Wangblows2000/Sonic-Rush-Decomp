// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

asm void func_02027fc8(void)
{
    ands r12, r1, #0x20000000
    movne r0, r2
    bxne lr
    add r0, r0, r2
    ands r2, r1, #0xc0000000
    beq _cc
    ldr r2, [pc, #0xe8]
    ands r12, r1, #0x40000000
    and r2, r1, r2
    beq _50
    ands r1, r3, #1
    beq _48
    cmp r3, r2
    movhi r1, r2, lsr #1
    addhi r3, r1, #1
    bhi _cc
    mov r2, r3, lsr #1
    b _d4
_48:
    mov r3, r3, lsr #1
    b _cc
_50:
    ands r1, r3, #3
    beq _c8
    cmp r3, r2
    addhi r3, r1, r2, lsr #2
    bhi _cc
    ands r1, r3, #1
    beq _c0
    ands r1, r3, #2
    movne r1, r3, lsr #2
    addne r2, r1, #1
    moveq r2, r3, lsr #2
    addeq r1, r2, #1
    mov r12, r2, lsl #2
    add r3, r0, r2, lsl #2
    mov r2, r1, lsl #2
    add r1, r0, r1, lsl #2
    ldrsh r12, [r0, r12]
    ldrsh r2, [r0, r2]
    mov r0, #3
    ldrsh r3, [r3, #2]
    ldrsh r1, [r1, #2]
    mla r2, r12, r0, r2
    mla r1, r3, r0, r1
    ldr r0, [pc, #0x54]
    mov r1, r1, asr #2
    and r0, r0, r2, asr #2
    orr r0, r0, r1, lsl #16
    bx lr
_c0:
    mov r2, r3, lsr #2
    b _d4
_c8:
    mov r3, r3, lsr #2
_cc:
    ldr r0, [r0, r3, lsl #2]
    bx lr
_d4:
    add r1, r0, r2, lsl #2
    mov r2, r2, lsl #2
    ldrsh r12, [r0, r2]
    ldrsh r3, [r1, #4]
    ldrsh r2, [r1, #2]
    ldrsh r1, [r1, #6]
    ldr r0, [pc, #0x14]
    add r3, r12, r3
    add r1, r2, r1
    and r2, r0, r3, asr #1
    mov r0, r1, asr #1
    orr r0, r2, r0, lsl #16
    bx lr
    dcd 0x0000ffff
}
