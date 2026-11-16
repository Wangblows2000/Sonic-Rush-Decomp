// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_0207ba24;
extern void func_020337ec(void);

asm void func_020551b0(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    mov r4, r0
    ldrsh r0, [r4]
    ldr r2, [pc, #0xec]
    ldr r3, [pc, #0xec]
    eor r6, r0, #1
    add r0, r0, #2
    add r1, r6, #2
    mov r0, r0, lsl #0x10
    mov r1, r1, lsl #0x10
    add r2, r4, r2
    mov r12, r0, asr #0x10
    mov r5, r1, asr #0x10
    add r6, r2, r6, lsl #9
    add lr, r4, r3
    mov r3, #0
    ldr r0, [pc, #0xc0]
    mov r2, r3
_48:
    ldrh r1, [r6]
    add r3, r3, #1
    mov r7, r3, lsl #0x10
    and r3, r1, #0xf
    ldrb r3, [r0, r3]
    and r9, r1, #0xf0
    and r8, r1, #0xf00
    orr r3, r2, r3
    mov r3, r3, lsl #0x10
    ldrb r10, [r0, r9, asr #4]
    mov r3, r3, lsr #0x10
    ldrb r9, [r0, r8, asr #8]
    orr r3, r3, r10, lsl #4
    mov r3, r3, lsl #0x10
    mov r3, r3, lsr #0x10
    and r8, r1, #0xf000
    orr r1, r3, r9, lsl #8
    mov r1, r1, lsl #0x10
    mov r3, r7, lsr #0x10
    ldrb r7, [r0, r8, asr #12]
    mov r1, r1, lsr #0x10
    cmp r3, #0x100
    orr r1, r1, r7, lsl #12
    strh r1, [lr]
    add r6, r6, #2
    add lr, lr, #2
    blo _48
    mov r0, #0x48
    smlabb r1, r12, r0, r4
    ldr r0, [pc, #0x3c]
    ldrsh r6, [r4]
    add r0, r4, r0
    ldr r3, [r1, #0x54]
    add r0, r0, r6, lsl #9
    mov r1, #0x200
    bl func_020337ec
    mov r0, #0x48
    smlabb r1, r5, r0, r4
    ldr r0, [pc, #0x1c]
    ldr r3, [r1, #0x54]
    add r0, r4, r0
    mov r1, #0x200
    mov r2, #0
    bl func_020337ec
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    bx lr
    dcd 0x0000017a
    dcd 0x0000057a
    dcd data_0207ba24
}
