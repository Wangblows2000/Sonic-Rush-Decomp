// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void _s32_div_f(void);
extern u32 data_ov001_02316028;
extern void AnimatorSprite_Draw(void);
extern void AnimatorSprite_ProcessAnimation(void);
extern void AnimatorSprite_SetAnimation(void);

#pragma thumb on
asm void OptionsMenu_Unk_ov001_022c9a2c(void)
{
    push {r4, r5, r6, r7, lr}
    sub sp, #0x24
    str r2, [sp]
    ldr r2, [pc, #0x134]
    ldr r2, [r0, r2]
    str r2, [sp, #8]
    add r4, r0, #0
    add r4, #0x38
    str r1, [sp, #0xc]
    sub r1, #0x27
    str r1, [sp, #0xc]
    ldr r0, [r0, #0x38]
    cmp r0, #0
    bne _26
    add r0, r4, #0
    str r0, [sp, #4]
    add r0, #0x38
    str r0, [sp, #4]
    b _2e
_26:
    add r0, r4, #0
    str r0, [sp, #4]
    add r0, #0xdc
    str r0, [sp, #4]
_2e:
    mov r0, #0
    str r0, [sp, #0x10]
    str r0, [sp, #0x14]
    lsl r2, r0, #0
    ldr r1, [sp, #4]
    ldr r0, [pc, #0x104]
_3a:
    ldr r3, [r1]
    cmp r3, #0x2e
    beq _58
    lsl r3, r3, #2
    ldr r5, [r0, r3]
    ldr r3, [sp, #0x14]
    add r3, r3, r5
    str r3, [sp, #0x14]
    ldr r3, [sp, #0x10]
    add r3, r3, #1
    str r3, [sp, #0x10]
    add r1, r1, #4
    add r2, r2, #1
    cmp r2, #0x29
    blt _3a
_58:
    ldr r0, [sp, #0x14]
    add r0, #0x10
    str r0, [sp, #0x14]
    cmp r0, #0x5e
    bge _66
    mov r0, #0x5e
    str r0, [sp, #0x14]
_66:
    ldr r0, [r4, #0x24]
    sub r0, #0x10
    cmp r0, #0
    bge _70
    mov r0, #0
_70:
    lsl r5, r0, #0xb
    cmp r5, #0
    ble _88
    add r0, r5, #0
    ldr r1, [sp, #0x14]
    lsl r1, r1, #0xc
    bl _s32_div_f
    cmp r1, #0
    bne _88
    mov r0, #0x10
    str r0, [r4, #0x24]
_88:
    asr r0, r5, #0xc
    ldr r1, [sp, #0x14]
    bl _s32_div_f
    ldr r0, [sp, #0xc]
    sub r0, r0, r1
    str r0, [sp, #0x1c]
    lsl r0, r0, #0x10
    asr r5, r0, #0x10
    ldr r1, [sp, #0xc]
    add r1, #0x4e
    ldr r0, [sp, #0x1c]
    cmp r0, r1
    bge _134
_a4:
    mov r0, #0
    str r0, [sp, #0x20]
    ldr r0, [sp, #0x10]
    cmp r0, #0
    ble _11e
    ldr r1, [sp, #8]
    ldr r0, [pc, #0x90]
    add r4, r1, r0
    ldr r6, [sp, #4]
    lsl r7, r1, #0
_b8:
    ldr r0, [sp, #0xc]
    sub r0, #8
    cmp r5, r0
    blt _f6
    ldr r0, [r6]
    lsl r0, r0, #0x10
    lsr r0, r0, #0x10
    str r0, [sp, #0x18]
    ldr r0, [pc, #0x7c]
    ldr r1, [r7, r0]
    ldr r0, [sp, #0x18]
    cmp r0, r1
    beq _e0
    add r0, r4, #0
    ldr r1, [sp, #0x18]
    bl AnimatorSprite_SetAnimation
    ldr r1, [sp, #0x18]
    ldr r0, [pc, #0x68]
    str r1, [r7, r0]
_e0:
    strh r5, [r4, #4]
    ldr r0, [sp]
    strh r0, [r4, #6]
    add r0, r4, #0
    mov r1, #0
    lsl r2, r1, #0
    bl AnimatorSprite_ProcessAnimation
    add r0, r4, #0
    bl AnimatorSprite_Draw
_f6:
    ldr r0, [r6]
    lsl r1, r0, #2
    ldr r0, [pc, #0x44]
    ldr r0, [r0, r1]
    add r0, r5, r0
    lsl r0, r0, #0x10
    asr r5, r0, #0x10
    ldr r0, [sp, #0xc]
    add r0, #0x4e
    cmp r5, r0
    bge _11e
    add r4, #0x48
    add r6, r6, #4
    add r7, r7, #4
    ldr r0, [sp, #0x20]
    add r1, r0, #1
    str r1, [sp, #0x20]
    ldr r0, [sp, #0x10]
    cmp r1, r0
    blt _b8
_11e:
    ldr r1, [sp, #0x1c]
    ldr r0, [sp, #0x14]
    add r0, r1, r0
    str r0, [sp, #0x1c]
    lsl r0, r0, #0x10
    asr r5, r0, #0x10
    ldr r1, [sp, #0xc]
    add r1, #0x4e
    ldr r0, [sp, #0x1c]
    cmp r0, r1
    blt _a4
_134:
    add sp, #0x24
    pop {r4, r5, r6, r7}
    pop {r3}
    bx r3
    dcd 0x000003ac
    dcd data_ov001_02316028
    dcd 0x00000c0c
    dcd 0x00001794
}
#pragma thumb reset
