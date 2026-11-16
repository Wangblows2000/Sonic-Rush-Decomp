// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void _s32_div_f(void);
extern void ObjSpd_DownSet(void);
extern void ObjSpd_UpSet(void);
extern void Player_ApplyVerticalInput(void);
extern void Player_UpdateHighSpeedTimer(void);

asm void Player_ApplyAirInput(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, lr}
    mov r8, r0
    add r0, r8, #0x300
    ldrsh r4, [r0, #0x5a]
    ldrsh r6, [r0, #0x56]
    ldrsh r7, [r0, #0x58]
    mov r1, #0
    mov r5, r4
    strh r1, [r0, #0x72]
    ldr r2, [r8, #0x32c]
    ands r1, r2, #0x100
    ldrnesh r7, [r0, #0x64]
    ands r0, r2, #0x80
    addne r0, r8, #0x300
    ldrnesh r6, [r0, #0x66]
    ldrnesh r5, [r0, #0x6a]
    ldrnesh r7, [r0, #0x68]
    mov r0, r8
    bl Player_UpdateHighSpeedTimer
    ldrb r1, [r8, #1]
    add r0, r1, #0x20
    ands r0, r0, #0xc0
    bne _64
    cmp r1, #0xe0
    bne _6c
_64:
    mov r0, r5, lsl #0xe
    mov r5, r0, asr #0x10
_6c:
    add r0, r8, #0x300
    ldrsh r0, [r0, #0x52]
    cmp r0, #0
    movne r0, r6, lsl #0xe
    movne r6, r0, asr #0x10
    movne r5, #0
    bne _f4
    ldrsh r12, [r8, #0x2e]
    add r0, r8, #0x300
    ldrsh r2, [r0, #0x7a]
    cmp r12, #0
    rsblt r3, r12, #0
    movge r3, r12
    mov r1, #0
    cmp r3, r2
    ble _e4
    cmp r12, #0
    rsblt r12, r12, #0
    sub r0, r12, r2
    mov r0, r0, lsl #8
    sub r1, r7, r2
    bl _s32_div_f
    mov r0, r0, lsl #0x10
    mov r1, r0, asr #0x10
    cmp r1, #0x100
    movgt r1, #0x100
    mov r0, #0xf8
    smulbb r0, r1, r0
    mov r0, r0, lsl #8
    mov r1, r0, asr #0x10
_e4:
    mul r0, r6, r1
    sub r0, r6, r0, asr #8
    mov r0, r0, lsl #0x10
    mov r6, r0, asr #0x10
_f4:
    ldr r0, [r8, #0x32c]
    ands r0, r0, #0x4000000
    add r0, r8, #0x300
    ldrsh r0, [r0, #0xc4]
    movne r6, r6, asr #1
    movne r5, r5, asr #1
    cmp r0, #0
    ldmneia sp!, {r4, r5, r6, r7, r8, lr}
    bxne lr
    add r0, r8, #0x400
    ldrh r1, [r0, #0x20]
    ands r0, r1, #0x30
    beq _1e8
    ands r0, r1, #0x10
    beq _18c
    ldrh r0, [r8, #0x5c]
    bic r0, r0, #1
    strh r0, [r8, #0x5c]
    ldrsh r0, [r8, #0x2e]
    cmp r0, #0
    bge _158
    mov r1, r5, lsl #0x11
    mov r1, r1, asr #0x10
    bl ObjSpd_DownSet
    strh r0, [r8, #0x2e]
_158:
    ldrsh r0, [r8, #0x34]
    cmp r0, #0
    bge _174
    mov r1, r4, lsl #0x11
    mov r1, r1, asr #0x10
    bl ObjSpd_DownSet
    strh r0, [r8, #0x34]
_174:
    ldrsh r0, [r8, #0x2e]
    mov r1, r6
    mov r2, r7
    bl ObjSpd_UpSet
    strh r0, [r8, #0x2e]
    b _250
_18c:
    ldrh r0, [r8, #0x5c]
    orr r0, r0, #1
    strh r0, [r8, #0x5c]
    ldrsh r0, [r8, #0x2e]
    cmp r0, #0
    ble _1b4
    mov r1, r5, lsl #0x11
    mov r1, r1, asr #0x10
    bl ObjSpd_DownSet
    strh r0, [r8, #0x2e]
_1b4:
    ldrsh r0, [r8, #0x34]
    cmp r0, #0
    ble _1d0
    mov r1, r4, lsl #0x11
    mov r1, r1, asr #0x10
    bl ObjSpd_DownSet
    strh r0, [r8, #0x34]
_1d0:
    ldrsh r0, [r8, #0x2e]
    mov r2, r7
    rsb r1, r6, #0
    bl ObjSpd_UpSet
    strh r0, [r8, #0x2e]
    b _250
_1e8:
    ldrsh r0, [r8, #0x2e]
    rsb r1, r7, #0
    cmp r0, r1
    movlt r0, r1
    blt _204
    cmp r0, r7
    movgt r0, r7
_204:
    strh r0, [r8, #0x2e]
    ldrsh r0, [r8, #0x34]
    cmp r0, r1
    blt _220
    cmp r0, r7
    movle r7, r0
    mov r1, r7
_220:
    strh r1, [r8, #0x34]
    add r0, r8, #0x300
    mov r1, #0
    strh r1, [r0, #0xac]
    ldrsh r0, [r8, #0x2e]
    mov r1, r5
    bl ObjSpd_DownSet
    strh r0, [r8, #0x2e]
    ldrsh r0, [r8, #0x34]
    mov r1, r4
    bl ObjSpd_DownSet
    strh r0, [r8, #0x34]
_250:
    mov r0, r8
    bl Player_ApplyVerticalInput
    ldmia sp!, {r4, r5, r6, r7, r8, lr}
    bx lr
}
