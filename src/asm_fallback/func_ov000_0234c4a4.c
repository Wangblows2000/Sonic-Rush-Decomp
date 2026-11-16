// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_ov000_02368bdc;
extern u32 data_ov000_02368c44;
extern u32 data_ov000_02368cac;
extern u32 data_ov000_02368d14;
extern void func_02031a68(void);
extern void func_02031bd0(void);
extern void func_02033550(void);

asm void func_ov000_0234c4a4(void)
{
    stmdb sp!, {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r5, r0
    add r0, r5, #0x300
    ldrsh r3, [r0, #0x28]
    mov r4, r1
    strh r4, [r0, #0x28]
    ldrb r1, [r5, #0x324]
    cmp r1, #0
    beq _50
    cmp r4, #0x30
    movge r0, #0x1b
    mlage r0, r1, r0, r4
    movge r0, r0, lsl #0x10
    movge r4, r0, asr #0x10
    cmp r3, #0x30
    movge r0, #0x1b
    mlage r0, r1, r0, r3
    movge r0, r0, lsl #0x10
    movge r3, r0, asr #0x10
_50:
    ldrh r1, [r5, #0x5c]
    add r0, r5, #0x200
    bic r1, r1, #8
    strh r1, [r5, #0x5c]
    ldrh r1, [r5, #0x5c]
    bic r1, r1, #4
    strh r1, [r5, #0x5c]
    ldrh r1, [r0, #0xaa]
    bic r1, r1, #4
    strh r1, [r0, #0xaa]
    ldrh r1, [r0, #0xd6]
    bic r1, r1, #4
    strh r1, [r0, #0xd6]
    ldr r0, [r5, #0x124]
    cmp r0, #0
    beq _158
    ldr r1, [pc, #0xf8]
    ldrb r2, [r1, r4]
    ldrb r1, [r1, r3]
    cmp r1, r2
    beq _d4
    ldr r1, [r0, #0x138]
    ldr r7, [r5, #0x1f8]
    ldr r6, [r5, #0x1fc]
    bl func_02031bd0
    ldrh r0, [r5, #0x5c]
    bic r0, r0, #0x400
    strh r0, [r5, #0x5c]
    str r7, [r5, #0x1f8]
    str r6, [r5, #0x1fc]
    ldr r0, [r5, #0x32c]
    orr r0, r0, #0x80000000
    str r0, [r5, #0x32c]
_d4:
    ldrh r0, [r5, #0x5c]
    ldr r3, [pc, #0xb4]
    ands r0, r0, #0x400
    ldrne r1, [r5, #0x124]
    ldrneb r0, [r1, #0x10c]
    orrne r0, r0, #0x40
    strneb r0, [r1, #0x10c]
    ldreq r1, [r5, #0x124]
    ldreqb r0, [r1, #0x10c]
    biceq r0, r0, #0x40
    streqb r0, [r1, #0x10c]
    ldr r0, [r5, #0x124]
    mov r1, #0
    str r1, [sp]
    ldrb r3, [r3, r4]
    ldr r2, [r0, #0x13c]
    bl func_02031a68
    ldr r0, [pc, #0x78]
    ldrb r3, [r0, r4]
    cmp r3, #0
    beq _140
    ldr r0, [r5, #0x124]
    mov r1, #0
    str r1, [sp]
    ldr r2, [r0, #0x14c]
    mov r1, #4
    bl func_02031a68
_140:
    ldr r0, [r5, #0x124]
    mov r1, #0x1000
    str r1, [r0, #0x114]
    ldrh r0, [r5, #0x5c]
    bic r0, r0, #0x400
    strh r0, [r5, #0x5c]
_158:
    mov r1, #0x10
    strh r1, [r5, #0xbe]
    add r0, r5, #0x100
    ldr r6, [pc, #0x30]
    strh r1, [r0, #6]
    ldrb r1, [r6, r4]
    add r0, r5, #0x94
    bl func_02033550
    ldrb r1, [r6, r4]
    add r0, r5, #0xdc
    bl func_02033550
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
    dcd data_ov000_02368c44
    dcd data_ov000_02368bdc
    dcd data_ov000_02368cac
    dcd data_ov000_02368d14
}
