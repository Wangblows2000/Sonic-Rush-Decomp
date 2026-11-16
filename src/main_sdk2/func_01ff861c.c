#include <nitro/types.h>

extern u32 data_02072c68;
extern u32 data_02087cc4;
extern u32 data_02087cdc;
extern u32 data_027e0000;
void DC_StoreRange(void);
void MIi_CpuCopy16(void);
void MI_StopDma(void);
void MI_HBlankDmaCopy16(void);
void MI_HBlankDmaCopy32(void);

asm void func_01ff861c(void)
{
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    ldr r0, =data_02087cc4
    ldr r5, =data_02087cc4
    ldr r2, [r0]
    ldr r8, =data_02087cdc
    ands r0, r2, #0x1000
    ldrne r0, =data_02087cdc
    ldr r11, =data_02072c68
    ldrne r1, [r0, #0x10]
    mov r10, #0
    eorne r1, r1, #1
    strne r1, [r0, #0x10]
    ands r0, r2, #0x2000
    ldrne r0, =data_02087cdc
    mov r6, #0x400
    ldrne r1, [r0, #0x24]
    mov r4, #0xc0
    eorne r1, r1, #1
    strne r1, [r0, #0x24]
    ldr r0, [r5]
    bic r0, r0, #0x3000
    str r0, [r5]
@1:
    ldr r7, [r11, r10, lsl #2]
    mov r0, r7
    bl MI_StopDma
    mov r0, r6, lsl r10
    ldr r1, [r5]
    ands r0, r1, r0
    beq @4
    ldr r0, [r8, #0x10]
    ldr r1, [r8]
    eor r9, r0, #1
    add r0, r8, r9, lsl #2
    ldr r0, [r0, #4]
    ldr r2, [r8, #0xc]
    bl MIi_CpuCopy16
    ldr r3, [r8, #0xc]
    ands r0, r3, #3
    beq @2
    add r0, r8, r9, lsl #2
    ldr r1, [r0, #4]
    ldr r2, [r8]
    mov r0, r7
    add r1, r3, r1
    bl MI_HBlankDmaCopy16
    b @3
@2:
    add r0, r8, r9, lsl #2
    ldr r1, [r0, #4]
    ldr r2, [r8]
    mov r0, r7
    add r1, r3, r1
    bl MI_HBlankDmaCopy32
@3:
    ldr r0, [r8, #0xc]
    add r2, r8, r9, lsl #2
    mul r1, r0, r4
    ldr r0, [r2, #4]
    bl DC_StoreRange
@4:
    add r10, r10, #1
    cmp r10, #2
    add r8, r8, #0x14
    blo @1
    ldr r0, =data_027e0000
    add r0, r0, #0x3000
    ldr r1, [r0, #0xff8]
    orr r1, r1, #1
    str r1, [r0, #0xff8]
    add sp, sp, #4
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
}

