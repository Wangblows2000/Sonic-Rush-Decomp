// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_ov000_02364d54;
extern u32 data_ov000_023687d4;
extern u32 data_ov000_0236aa88;
extern u32 data_ov000_0236aad8;
extern u32 data_ov000_0236abd8;

asm void func_ov000_023350bc(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x44
    ldr r4, [pc, #0x1e8]
    mov r3, #0x64
    mla r11, r1, r3, r4
    ldr r3, [r11, #4]
    ldr r1, [pc, #0x1dc]
    mov r3, r3, asr #8
    str r3, [sp, #0x30]
    ldr r4, [r11, #8]
    ldrh r3, [r1, #0xe]
    mov r4, r4, asr #8
    str r4, [sp, #0x34]
    ldrsh r4, [r11, #0x1e]
    ldr r5, [r11, #4]
    mov r10, r0
    add r4, r5, r4, lsl #8
    mov r4, r4, asr #8
    str r4, [sp, #0x38]
    ldrsh r5, [r11, #0x20]
    ldr r6, [r11, #8]
    mov r4, #0xc0
    smlabb r4, r5, r4, r6
    sub r0, r3, #1
    mov r4, r4, asr #8
    str r4, [sp, #0x3c]
    ldrh r1, [r1, #0x10]
    str r0, [sp, #0x10]
    ldr r4, [pc, #0x188]
    sub r0, r1, #1
    str r0, [sp, #0x14]
    mov r0, #0
    str r2, [sp, #8]
    str r0, [sp, #0xc]
    add r6, sp, #0x30
    add r5, sp, #0x20
    str r0, [sp, #0x18]
    str r0, [sp, #0x1c]
_98:
    ldr r1, [pc, #0x164]
    ldr r0, [sp, #0xc]
    ldr r9, [sp, #8]
    ldr r2, [r1, r0, lsl #2]
    ldr r1, [pc, #0x158]
    ldr r0, [r1, r0, lsl #2]
    ldrh r1, [r2]
    mov r2, #0xc0
    str r0, [r4]
    ldr r0, [r11, #4]
    mov r0, r0, asr #8
    sub r0, r0, r1
    str r0, [sp, #0x20]
    ldr r3, [r11, #8]
    mov r3, r3, asr #8
    sub r3, r3, r1
    str r3, [sp, #0x24]
    ldrsh r3, [r11, #0x1e]
    ldr r7, [r11, #4]
    add r3, r7, r3, lsl #8
    add r3, r1, r3, asr #8
    str r3, [sp, #0x28]
    ldrsh r3, [r11, #0x20]
    ldr r7, [r11, #8]
    smlabb r2, r3, r2, r7
    cmp r0, #0
    ldrle r0, [sp, #0x18]
    add r1, r1, r2, asr #8
    strle r0, [sp, #0x20]
    movle r0, r9
    orrle r9, r0, #0x10
    ldr r0, [sp, #0x24]
    str r1, [sp, #0x2c]
    cmp r0, #0
    ldrle r0, [sp, #0x1c]
    ldr r1, [sp, #0x28]
    strle r0, [sp, #0x24]
    ldr r0, [sp, #0x10]
    orrle r9, r9, #0x20
    cmp r1, r0
    strge r0, [sp, #0x28]
    ldr r1, [sp, #0x2c]
    ldr r0, [sp, #0x14]
    orrge r9, r9, #0x40
    cmp r1, r0
    strge r0, [sp, #0x2c]
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x2c]
    mov r0, r0, lsl #8
    mov r7, r0, lsr #0x10
    orrge r9, r9, #0x80
    cmp r7, r1, asr #8
    bgt _1d4
_16c:
    ldr r0, [sp, #0x20]
    ldr r1, [sp, #0x28]
    mov r0, r0, lsl #8
    mov r8, r0, lsr #0x10
    cmp r8, r1, asr #8
    bgt _1bc
_184:
    str r6, [sp]
    str r9, [sp, #4]
    mov r0, r10
    mov r1, r8
    mov r2, r7
    mov r3, r5
    ldr r12, [r4]
    blx r12
    add r0, r8, #1
    mov r0, r0, lsl #0x10
    mov r8, r0, lsr #0x10
    ldr r0, [sp, #0x28]
    cmp r8, r0, asr #8
    ble _184
_1bc:
    add r0, r7, #1
    mov r0, r0, lsl #0x10
    mov r7, r0, lsr #0x10
    ldr r0, [sp, #0x2c]
    cmp r7, r0, asr #8
    ble _16c
_1d4:
    ldr r0, [sp, #0xc]
    add r0, r0, #1
    and r0, r0, #0xff
    str r0, [sp, #0xc]
    cmp r0, #3
    blo _98
    add sp, sp, #0x44
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_ov000_0236aad8
    dcd data_ov000_0236abd8
    dcd data_ov000_0236aa88
    dcd data_ov000_02364d54
    dcd data_ov000_023687d4
}
