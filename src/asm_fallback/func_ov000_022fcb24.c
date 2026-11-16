// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gPlayers;
extern void Sound_PlaySeqArcEx(void);
extern void Sound_SetPositional(void);
extern void func_ov000_022faf48(void);
extern void EnemyPawnGroup_State_Unk_ov000_022fcaa8(void);

asm void func_ov000_022fcb24(void)
{
    stmdb sp!, {r4, lr}
    mov r4, r0
    mov r1, #3
    bl func_ov000_022faf48
    ldr r1, [r4, #0x58]
    mov r0, #0x2c0
    orr r1, r1, #0x10
    str r1, [r4, #0x58]
    ldrh r1, [r4, #0x5c]
    ldr r2, [pc, #0xc4]
    rsb r0, r0, #0
    orr r1, r1, #4
    strh r1, [r4, #0x5c]
    ldr r3, [r4, #0x58]
    mov r1, #0x180
    bic r3, r3, #1
    str r3, [r4, #0x58]
    str r2, [r4, #0x68]
    strh r0, [r4, #0x30]
    ldr r12, [r4, #0x1e0]
    ldr r0, [r4, #0x1e8]
    ldrh r3, [r12, #4]
    ldrsb lr, [r12, #6]
    ldr r2, [r4, #8]
    ands r3, r3, #3
    addne r1, r1, r3, lsl #7
    ldrb r3, [r12, #8]
    movne r1, r1, lsl #0x10
    movne r1, r1, asr #0x10
    add r3, lr, r3
    mul r3, r1, r3
    mul r12, lr, r1
    add r1, r3, r0, lsl #8
    cmp r2, r1
    movgt r2, r1
    add r0, r12, r0, lsl #8
    cmp r2, r0
    movlt r2, r0
    ldr r1, [r4, #0x10]
    mov r0, #0x3c
    sub r1, r2, r1
    mov r1, r1, asr #5
    strh r1, [r4, #0x34]
    str r0, [r4, #8]
    mov r0, #0x20
    str r0, [r4, #0xc]
    ldr r0, [r4, #0x1f4]
    mov r1, #0
    bic r0, r0, #1
    str r0, [r4, #0x1f4]
    ldr r0, [r4, #0x1dc]
    mov r2, #0x91
    bl Sound_PlaySeqArcEx
    ldr r1, [pc, #0x18]
    ldr r0, [r4, #0x1dc]
    ldr r2, [r1]
    mov r1, r4
    bl Sound_SetPositional
    ldmia sp!, {r4, lr}
    bx lr
    dcd EnemyPawnGroup_State_Unk_ov000_022fcaa8
    dcd gPlayers
}
