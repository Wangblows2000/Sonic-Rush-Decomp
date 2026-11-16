// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void Model_SetAnimation(void);
extern void Sound_PlayVoice(void);
extern void Boss4_StepTiltToward(void);
extern void Boss4_SubState_SlamWaitAnim(void);
extern void Boss4_SetCameraZoomOut(void);

asm void Boss4_SubState_SlamStopSpin(void)
{
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r4, r0
    ldr r0, [r4, #0x224]
    cmp r0, #1
    bne _1c
    bl Boss4_SetCameraZoomOut
_1c:
    ldr r0, [r4, #0xbc0]
    sub r0, r0, #8
    str r0, [r4, #0xbc0]
    ldr r0, [r4, #0xbc0]
    cmp r0, #0
    movlt r0, #0
    strlt r0, [r4, #0xbc0]
    ldr r2, [r4, #0xbb8]
    ldr r1, [r4, #0xbc0]
    mov r0, r4
    add r1, r2, r1
    str r1, [r4, #0xbb8]
    ldr r1, [r4, #0xbc8]
    bl Boss4_StepTiltToward
    ldr r0, [r4, #0xbc0]
    cmp r0, #0
    addne sp, sp, #4
    ldmneia sp!, {r4, r5, lr}
    bxne lr
    ldr r0, [r4, #0xbc4]
    cmp r0, #0
    addne sp, sp, #4
    ldmneia sp!, {r4, r5, lr}
    bxne lr
    ldr r0, [pc, #0x48]
    mov r1, #0
    str r1, [sp]
    add r5, r4, r0
    ldr r2, [r4, #0xab0]
    mov r0, r5
    mov r3, #4
    bl Model_SetAnimation
    mov r2, #0x1000
    str r2, [r5, #0x118]
    ldr r1, [pc, #0x24]
    mov r0, #0
    str r1, [r4, #0x220]
    mov r1, r0
    mov r2, #0xd2
    str r0, [r4, #0x224]
    bl Sound_PlayVoice
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
    dcd 0x00000708
    dcd Boss4_SubState_SlamWaitAnim
}
