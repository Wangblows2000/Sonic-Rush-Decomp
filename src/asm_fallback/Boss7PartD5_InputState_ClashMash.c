// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_0206e470;
extern u32 gRandSeed;
extern u32 gGameState;
extern u32 data_ov000_02363b18;
extern u32 gBoss7;
extern void FX_Div(void);

asm void Boss7PartD5_InputState_ClashMash(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, lr}
    ldr r1, [pc, #0x1c8]
    mov r7, r0
    ldr r6, [r1]
    mov r5, #0
    ldr r0, [r6, #0x220]
    ands r0, r0, #1
    beq _1c8
    ldr r0, [pc, #0x1b0]
    mov r1, #1
    ldr r0, [r0, #0x10]
    cmp r0, #2
    beq _3c
    cmp r0, #3
    movne r1, r5
_3c:
    cmp r1, #0
    movne r0, #1
    ldreq r0, [pc, #0x18c]
    ldr r1, [pc, #0x18c]
    ldreq r0, [r0, #0x14]
    mov r2, r0, lsl #1
    ldr r0, [r7, #0x444]
    ldrh r4, [r1, r2]
    add r1, r0, #1
    ldr r0, [pc, #0x178]
    str r1, [r7, #0x444]
    ldr r1, [r7, #0x444]
    cmp r1, r0
    movhi r1, r0
    str r1, [r7, #0x444]
    ldr r0, [r6, #0x384]
    mov r1, #0x8000000
    add r0, r0, #0x4000
    mov r0, r0, lsl #0xc
    bl FX_Div
    ldr r3, [r7, #0x444]
    ldr r1, [pc, #0x148]
    ldr r2, [pc, #0x148]
    sub r1, r1, r3
    mul r2, r1, r2
    ldr r7, [pc, #0x140]
    ldr lr, [r6, #0x220]
    smull r1, r3, r7, r2
    add r3, r2, r3
    mov r1, #0xa
    mul r7, r4, r1
    add r1, r6, #0x200
    ldrh r6, [r1, #0x2e]
    mov r1, r7, asr #3
    add r1, r7, r1, lsr #28
    add r12, r6, lr, lsl #5
    mov r3, r3, asr #0xa
    mov r2, r2, lsr #0x1f
    add r3, r2, r3
    mov r2, #6
    mul r7, r4, r2
    ldr r6, [pc, #0x104]
    mov r2, r7, asr #3
    add r6, r3, r6
    add r2, r7, r2, lsr #28
    mov r7, r12, lsl #0x10
    ldr r8, [pc, #0xf4]
    mov r4, lr, lsl #0x16
    and r4, r8, r4, lsr #16
    and r7, r8, r7, lsr #16
    mov r8, r7, asr #4
    mov r4, r4, asr #4
    smull r6, r12, r0, r6
    mov r7, #0x800
    adds r0, r6, r7
    ldr r6, [pc, #0xd4]
    mov r4, r4, lsl #2
    ldrsh lr, [r6, r4]
    mov r4, r8, lsl #2
    ldrsh r6, [r6, r4]
    add r4, lr, #0x1000
    adc r8, r12, #0
    mov r12, r0, lsr #0xc
    add r4, r4, r4, lsr #31
    add r6, r6, #0x1000
    orr r12, r12, r8, lsl #20
    mov r0, r4, asr #1
    smull r4, r0, r12, r0
    adds r4, r4, r7
    adc r0, r0, #0
    mov r4, r4, lsr #0xc
    mov r1, r1, asr #4
    orr r4, r4, r0, lsl #20
    mul r8, r1, r4
    add r6, r6, r6, lsr #31
    mov r0, r6, asr #1
    smull r0, r4, r8, r0
    adds r7, r0, r7
    ldr r3, [pc, #0x7c]
    ldr r0, [pc, #0x7c]
    ldr r8, [r3]
    ldr r1, [pc, #0x78]
    adc r6, r4, #0
    mla r0, r8, r0, r1
    mov r1, r0, lsr #0x10
    mov r7, r7, lsr #0xc
    mov r1, r1, lsl #0x10
    mov r4, r1, lsr #0x10
    mov r1, r2, asr #4
    orr r7, r7, r6, lsl #20
    add r1, r1, r7, asr #12
    mov r1, r1, lsl #0x10
    mov r2, r1, lsr #0x10
    and r1, r4, #0xff
    str r0, [r3]
    cmp r2, r1
    orrge r0, r5, #1
    movge r0, r0, lsl #0x10
    movge r5, r0, lsr #0x10
_1c8:
    mov r0, r5
    ldmia sp!, {r4, r5, r6, r7, r8, lr}
    bx lr
    dcd gBoss7
    dcd gGameState
    dcd data_ov000_02363b18
    dcd 0x00000708
    dcd 0x00000b33
    dcd 0x91a2b3c5
    dcd 0x000004cc
    dcd 0x0000ffff
    dcd data_0206e470
    dcd gRandSeed
    dcd 0x00196225
    dcd 0x3c6ef35f
}
