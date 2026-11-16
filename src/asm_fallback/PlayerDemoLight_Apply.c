// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_02074a04;
extern u32 data_020756dc;
extern u32 data_020766a4;
extern u32 data_0207c3a0;
extern u32 gPlayerDemoId;
extern void G3X_SetEdgeColorTable(void);
extern void NNS_G3dGlbLightColor(void);
extern void NNS_G3dGlbLightVector(void);
extern void PlayerDemoLight_GetColors(void);
extern void PlayerDemoLight_GetVectors(void);

#pragma thumb on
asm void PlayerDemoLight_Apply(void)
{
    push {r4, r5, r6, r7, lr}
    sub sp, #0x1c
    ldr r2, [pc, #0xd4]
    mov r1, #0
    ldrsh r4, [r2, r1]
    ldr r3, [pc, #0xd4]
    mov r1, #0xc
    add r2, r4, #0
    mul r2, r1
    add r1, r3, r2
    str r1, [sp]
    lsl r1, r4, #0x18
    lsr r1, r1, #0x18
    str r1, [sp, #4]
    ldrb r0, [r0, #9]
    str r0, [sp, #8]
    lsl r0, r1, #0
    ldr r1, [sp, #8]
    bl PlayerDemoLight_GetColors
    add r4, r0, #0
    mov r0, #0
    ldrh r1, [r4]
    bl NNS_G3dGlbLightColor
    mov r0, #1
    ldrh r1, [r4, #2]
    bl NNS_G3dGlbLightColor
    mov r0, #2
    ldrh r1, [r4, #4]
    bl NNS_G3dGlbLightColor
    ldr r0, [sp, #4]
    ldr r1, [sp, #8]
    bl PlayerDemoLight_GetVectors
    add r4, r0, #0
    mov r5, #0
    mov r7, #2
    mov r6, #4
_52:
    add r1, r5, #0
    mov r0, #6
    mul r1, r0
    add r3, r4, r1
    add r0, r5, #0
    ldrsh r1, [r4, r1]
    ldrsh r2, [r3, r7]
    ldrsh r3, [r3, r6]
    bl NNS_G3dGlbLightVector
    add r0, r5, #1
    lsl r0, r0, #0x18
    lsr r5, r0, #0x18
    cmp r5, #3
    blo _52
    add r3, sp, #0xc
    mov r0, #0
    strh r0, [r3]
    strh r0, [r3, #2]
    strh r0, [r3, #4]
    strh r0, [r3, #6]
    strh r0, [r3, #8]
    strh r0, [r3, #0xa]
    strh r0, [r3, #0xc]
    strh r0, [r3, #0xe]
    mov r0, #8
    ldr r1, [sp]
    ldrsb r2, [r1, r0]
    mov r4, #9
    ldrsb r1, [r1, r4]
    ldr r4, [sp, #4]
    lsl r5, r4, #2
    ldr r4, [pc, #0x50]
    ldr r5, [r4, r5]
    ldr r4, [sp, #8]
    lsl r4, r4, #3
    ldr r4, [r5, r4]
    cmp r4, #0
    bne _ba
    ldr r5, [pc, #0x44]
    mov r4, #0x30
    mul r2, r4
    add r2, r5, r2
    lsl r1, r1, #4
    add r2, r2, r1
_ac:
    ldrb r1, [r2]
    add r2, r2, #1
    strb r1, [r3]
    add r3, r3, #1
    sub r0, r0, #1
    cmp r0, #0
    bne _ac
_ba:
    add r2, sp, #0x14
    ldr r1, [pc, #0x2c]
    ldrh r0, [r1]
    strh r0, [r2]
    ldrh r0, [r1, #2]
    strh r0, [r2, #2]
    ldrh r0, [r1, #4]
    strh r0, [r2, #4]
    ldrh r0, [r1, #6]
    strh r0, [r2, #6]
    add r0, sp, #0xc
    bl G3X_SetEdgeColorTable
    add sp, #0x1c
    pop {r4, r5, r6, r7}
    pop {r3}
    bx r3
    dcd gPlayerDemoId
    dcd data_020766a4
    dcd data_0207c3a0
    dcd data_02074a04
    dcd data_020756dc
}
#pragma thumb reset
