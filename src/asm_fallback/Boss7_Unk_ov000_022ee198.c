// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_ov000_02368bdc;
extern u32 data_ov000_02368c44;
extern u32 data_ov000_02368cac;
extern u32 data_ov000_02368d14;
extern void Model_SetAnimation(void);
extern void Model_SetResource(void);
extern void AnimatorSprite_SetAnimation(void);

asm void Boss7_Unk_ov000_022ee198(void)
{
    stmdb sp!, {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r5, r0
    ldr r3, [r5, #0x3d4]
    mov r4, r1
    str r4, [r5, #0x3d4]
    ldr r1, [r5, #0x3d0]
    cmp r1, #1
    bne _3c
    cmp r4, #0x30
    movge r0, #0x1b
    mlage r4, r1, r0, r4
    cmp r3, #0x30
    movge r0, #0x1b
    mlage r3, r1, r0, r3
_3c:
    ldrh r1, [r5, #0x5c]
    add r0, r5, #0x300
    bic r1, r1, #8
    strh r1, [r5, #0x5c]
    ldrh r1, [r5, #0x5c]
    bic r1, r1, #4
    strh r1, [r5, #0x5c]
    ldrh r1, [r0, #0x82]
    bic r1, r1, #4
    strh r1, [r0, #0x82]
    ldrh r1, [r0, #0xae]
    bic r1, r1, #4
    strh r1, [r0, #0xae]
    ldr r0, [r5, #0x124]
    cmp r0, #0
    beq _1a0
    ldr r1, [pc, #0x154]
    ldrb r2, [r1, r4]
    ldrb r1, [r1, r3]
    cmp r1, r2
    beq _b4
    ldr r1, [r0, #0x138]
    ldr r7, [r5, #0x2d0]
    ldr r6, [r5, #0x2d4]
    bl Model_SetResource
    ldrh r0, [r5, #0x5c]
    bic r0, r0, #0x400
    strh r0, [r5, #0x5c]
    str r7, [r5, #0x2d0]
    str r6, [r5, #0x2d4]
_b4:
    ldrh r0, [r5, #0x5c]
    ands r0, r0, #0x400
    ldrne r1, [r5, #0x124]
    ldrneb r0, [r1, #0x10c]
    orrne r0, r0, #0x40
    strneb r0, [r1, #0x10c]
    ldreq r1, [r5, #0x124]
    ldreqb r0, [r1, #0x10c]
    biceq r0, r0, #0x40
    streqb r0, [r1, #0x10c]
    cmp r4, #0x48
    blt _ec
    cmp r4, #0x4a
    ble _fc
_ec:
    cmp r4, #0x63
    blt _144
    cmp r4, #0x65
    bgt _144
_fc:
    mov r1, #0
    str r1, [sp]
    ldr r2, [pc, #0xd0]
    ldr r0, [r5, #0x124]
    ldrb r3, [r2, r4]
    ldr r2, [r5, #0x4cc]
    bl Model_SetAnimation
    ldr r0, [pc, #0xc0]
    ldrb r3, [r0, r4]
    cmp r3, #0
    beq _188
    mov r0, #0
    str r0, [sp]
    ldr r0, [r5, #0x124]
    ldr r2, [r5, #0x4d0]
    mov r1, #4
    bl Model_SetAnimation
    b _188
_144:
    ldr r0, [r5, #0x124]
    mov r1, #0
    str r1, [sp]
    ldr r3, [pc, #0x84]
    ldr r2, [r0, #0x13c]
    ldrb r3, [r3, r4]
    bl Model_SetAnimation
    ldr r0, [pc, #0x78]
    ldrb r3, [r0, r4]
    cmp r3, #0
    beq _188
    ldr r0, [r5, #0x124]
    mov r1, #0
    str r1, [sp]
    ldr r2, [r0, #0x14c]
    mov r1, #4
    bl Model_SetAnimation
_188:
    ldr r0, [r5, #0x124]
    mov r1, #0x1000
    str r1, [r0, #0x114]
    ldrh r0, [r5, #0x5c]
    bic r0, r0, #0x400
    strh r0, [r5, #0x5c]
_1a0:
    mov r1, #0x10
    strh r1, [r5, #0xbe]
    add r0, r5, #0x100
    ldr r6, [pc, #0x30]
    strh r1, [r0, #6]
    ldrb r1, [r6, r4]
    add r0, r5, #0x94
    bl AnimatorSprite_SetAnimation
    ldrb r1, [r6, r4]
    add r0, r5, #0xdc
    bl AnimatorSprite_SetAnimation
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
    dcd data_ov000_02368c44
    dcd data_ov000_02368bdc
    dcd data_ov000_02368cac
    dcd data_ov000_02368d14
}
