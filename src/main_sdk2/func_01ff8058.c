#include <nitro/types.h>

extern u32 data_0207d090;
extern u32 data_027e0000;
void func_0200d098(void);
void func_0200d104(void);

asm void func_01ff8058(void)
{
    ldr r12, =data_027e0000
    ldrh r3, [r12]
    mov r0, #0
    cmp r3, #0
    beq @2
    strh r0, [r12]
    ldr r12, =data_0207d090
    mov r1, #1
    strh r1, [r12]
    ldr r12, [r12, #0xc]
    mov r2, #1
@1:
    cmp r12, #0
    beq @2
    ldr r0, [r12, #0x6c]
    tst r3, r1, lsl r0
    strne r2, [r12, #0x64]
    ldr r12, [r12, #0x68]
    b @1
@2:
    ldr r12, =data_0207d090
    ldrh r1, [r12]
    cmp r1, #0
    ldreq pc, [sp], #4
    mov r1, #0
    strh r1, [r12]
    mov r3, #0xd2
    msr cpsr_c, r3
    add r2, r12, #0xc
    ldr r1, [r2]
@3:
    cmp r1, #0
    ldrneh r0, [r1, #0x64]
    cmpne r0, #1
    ldrne r1, [r1, #0x68]
    bne @3
    cmp r1, #0
    bne @5
@4:
    mov r3, #0x92
    msr cpsr_c, r3
    ldr pc, [sp], #4
@5:
    ldr r0, [r12, #8]
    cmp r1, r0
    beq @4
    ldr r3, [r12, #0x10]
    cmp r3, #0
    beq @6
    stmfd sp!, {r0, r1, r12}
    mov lr, pc
    bx r3
    ldmfd sp!, {r0, r1, r12}
@6:
    str r1, [r12, #8]
    mrs r2, spsr
    str r2, [r0, #0]!
    stmfd sp!, {r0, r1}
    add r0, r0, #0
    add r0, r0, #0x48
    ldr r1, =func_0200d104
    blx r1
    ldmfd sp!, {r0, r1}
    ldmib sp!, {r2, r3}
    stmib r0!, {r2, r3}
    ldmib sp!, {r2, r3, r12, lr}
    stmib r0!, {r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, sp, lr} ^
    stmib r0!, {lr}
    mov r3, #0xd3
    msr cpsr_c, r3
    stmib r0!, {sp}
    stmdb sp!, {r1}
    add r0, r1, #0
    add r0, r0, #0x48
    ldr r1, =func_0200d098
    blx r1
    ldmia sp!, {r1}
    ldr sp, [r1, #0x44]
    mov r3, #0xd2
    msr cpsr_c, r3
    ldr r2, [r1, #0]!
    msr spsr_fc, r2
    ldr lr, [r1, #0x40]
    ldmib r1!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, sp, lr} ^
    mov r0, r0
    stmda sp!, {r0, r1, r2, r3, r12, lr}
    ldmia sp!, {pc}
}

