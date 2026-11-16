#include <nitro/types.h>

extern u32 data_027e0000;
void OS_IrqHandler(void);
void func_02000900(void);
void func_02000914(void);
void func_020009b8(void);
void func_02000afc(void);
void _fp_init(void);
void __call_static_initializers(void);
void init_cp15(void);
void main(void);

asm void Entry(void)
{
    mov r12, #0x4000000
    str r12, [r12, #0x208]
    bl init_cp15
    mov r0, #0x13
    msr cpsr_c, r0
    ldr r0, =data_027e0000
    add r0, r0, #0x3fc0
    mov sp, r0
    mov r0, #0x12
    msr cpsr_c, r0
    ldr r0, =data_027e0000
    add r0, r0, #0x3fc0
    sub r0, r0, #0x40
    mov sp, r0
    ldr r1, =0x800
    sub r1, r0, r1
    mov r0, #0x1f
    msr cpsr_fsxc, r0
    sub sp, r1, #4
    mov r0, #0
    ldr r1, =data_027e0000
    mov r2, #0x4000
    bl func_02000900
    mov r0, #0
    ldr r1, =0x5000000
    mov r2, #0x400
    bl func_02000900
    mov r0, #0x200
    ldr r1, =0x7000000
    mov r2, #0x400
    bl func_02000900
    ldr r1, =0x2000b00
    ldr r0, [r1, #0x14]
    bl func_02000914
    bl func_020009b8
    ldr r0, =0x2000b00
    ldr r1, [r0, #0xc]
    ldr r2, [r0, #0x10]
    mov r0, #0
@1:
    cmp r1, r2
    strlo r0, [r1], #4
    blo @1
    ldr r1, =0x27fff9c
    str r0, [r1]
    ldr r1, =data_027e0000
    add r1, r1, #0x3fc0
    add r1, r1, #0x3c
    ldr r0, =OS_IrqHandler
    str r0, [r1]
    bl _fp_init
    bl func_02000afc
    bl __call_static_initializers
    ldr r1, =main
    ldr lr, =0xffff0000
    bx r1
}

