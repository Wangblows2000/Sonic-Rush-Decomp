#include <nitro/types.h>

extern u32 data_027e0000;
void OSi_CpuClear32(void);

asm void OSi_DoBoot(void)
{
    mov r12, #0x4000000
    str r12, [r12, #0x208]
    ldr r1, =data_027e0000
    add r1, r1, #0x3fc0
    add r1, r1, #0x3c
    mov r0, #0
    str r0, [r1]
    ldr r1, =0x4000180
@1:
    ldrh r0, [r1]
    and r0, r0, #0xf
    cmp r0, #1
    bne @1
    mov r0, #0x100
    strh r0, [r1]
    mov r0, #0
    ldr r3, =0x27ffd9c
    ldr r4, [r3]
    ldr r1, =0x27ffd80
    mov r2, #0x80
    bl OSi_CpuClear32
    str r4, [r3]
    ldr r1, =0x27fff80
    mov r2, #0x80
    bl OSi_CpuClear32
    ldr r1, =0x4000180
@2:
    ldrh r0, [r1]
    and r0, r0, #0xf
    cmp r0, #1
    beq @2
    mov r0, #0
    strh r0, [r1]
    ldr r3, =0x27ffe00
    ldr r12, [r3, #0x24]
    mov lr, r12
    ldr r11, =0x27fff80
    ldmia r11, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10}
    mov r11, #0
    bx r12
}

