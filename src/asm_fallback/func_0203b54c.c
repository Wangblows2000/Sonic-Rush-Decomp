// asm: fallback — C attempts did not match (best C draft: /home/claude/w_main_d3/b54c_g.c)
#include <nitro/types.h>

extern void OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(void);
extern u32 data_022b89e8;
extern void func_02016978(void);
extern void func_0201854c(void);
extern void func_0203b09c(void);

asm void func_0203b54c(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    mov r6, #0
    mov r7, #1
    ldr r9, [pc, #0xd0]
    mov r5, r7
    mov r4, #3
_18:
    ldrh r0, [r9, #0xa]
    mov r8, r5, lsl r7
    ands r0, r0, r8
    beq _a8
    mov r0, r7
    mov r1, r4
    bl func_02016978
    cmp r0, #0
    orrne r0, r6, r8
    movne r0, r0, lsl #0x10
    movne r6, r0, lsr #0x10
    bne _a8
    mvn r0, r8
    mov r8, r0, lsl #0x10
    bl OS_DisableInterrupts
    ldrh lr, [r9, #2]
    ldrh r12, [r9, #4]
    ldrh r3, [r9, #6]
    ldrh r2, [r9, #8]
    ldrh r1, [r9, #0xa]
    and lr, lr, r8, lsr #16
    and r12, r12, r8, lsr #16
    and r3, r3, r8, lsr #16
    and r2, r2, r8, lsr #16
    and r1, r1, r8, lsr #16
    ldrh r10, [r9, #0xc]
    strh lr, [r9, #2]
    strh r12, [r9, #4]
    and r8, r10, r8, lsr #16
    strh r3, [r9, #6]
    strh r2, [r9, #8]
    strh r1, [r9, #0xa]
    strh r8, [r9, #0xc]
    bl OS_RestoreInterrupts
    mov r0, r7
    bl func_0201854c
_a8:
    add r0, r7, #1
    mov r0, r0, lsl #0x10
    mov r7, r0, lsr #0x10
    cmp r7, #0x10
    blo _18
    cmp r6, #0
    bne _d4
    mov r0, #7
    bl func_0203b09c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    bx lr
_d4:
    mov r0, #4
    bl func_0203b09c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    bx lr
    dcd data_022b89e8
}
