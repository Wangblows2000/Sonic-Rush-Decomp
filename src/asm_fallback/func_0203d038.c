// asm: fallback — C attempts did not match (best C draft: /home/claude/w_main_d3/d038_p.c)
#include <nitro/types.h>

extern u32 data_022b8be0;
extern void func_02014a8c(void);
extern void func_02014b14(void);
extern void func_0203d190(void);

asm void func_0203d038(void)
{
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r3, [pc, #0x104]
    ldr r1, [r3, #0xc]
    add r2, r3, #0x440
    cmp r1, #0
    addeq sp, sp, #4
    ldmeqia sp!, {lr}
    bxeq lr
    ldrh r1, [r0, #4]
    cmp r1, #7
    beq _4c
    cmp r1, #9
    beq _84
    cmp r1, #0x15
    beq _c0
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
_4c:
    ldr r0, [r3]
    cmp r0, #0
    addeq sp, sp, #4
    ldmeqia sp!, {lr}
    bxeq lr
    ldr r0, [r3, #8]
    cmp r0, #0
    addne sp, sp, #4
    ldmneia sp!, {lr}
    bxne lr
    bl func_0203d190
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
_84:
    ldr r1, [r3]
    cmp r1, #0
    addeq sp, sp, #4
    ldmeqia sp!, {lr}
    bxeq lr
    ldrh r1, [r0, #0x12]
    add r0, r2, #0x8000
    mov r2, #1
    mvn r1, r2, lsl r1
    ldr r2, [r0, #0x418]
    add sp, sp, #4
    and r1, r2, r1
    str r1, [r0, #0x418]
    ldmia sp!, {lr}
    bx lr
_c0:
    ldr r1, [r3]
    ldr lr, [r0, #0xc]
    cmp r1, #0
    ldrh r1, [r0, #0x10]
    beq _100
    add r3, r2, #0x8000
    ldrh r2, [r0, #0x12]
    ldr r12, [r3, #0x418]
    mov r0, #1
    orr r12, r12, r0, lsl r2
    mov r0, lr
    str r12, [r3, #0x418]
    bl func_02014b14
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
_100:
    mov r0, lr
    bl func_02014a8c
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
    dcd data_022b8be0
}
