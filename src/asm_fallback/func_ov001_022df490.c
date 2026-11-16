// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

asm void func_ov001_022df490(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    mov r4, #0
    mov r1, r4
    add lr, r0, #0x54
    mov r5, r4
_14:
    str r5, [lr, r1, lsl #3]
    add r0, lr, r1, lsl #3
    add r1, r1, #1
    str r5, [r0, #4]
    cmp r1, #0x80
    blt _14
    ldr r7, [pc, #0x64]
    ldr r8, [pc, #0x64]
    mov r12, r5
    mov r0, #0
_3c:
    and r1, r5, #0xff
    mov r6, r0
    mov r3, r0
    orr r2, r1, r8
    mov r1, r12, lsl #5
_50:
    and r9, r6, r7
    add r10, r3, r1
    orr r9, r2, r9, lsl #16
    add r6, r6, #0x40
    str r9, [lr, r4, lsl #3]
    add r9, lr, r4, lsl #3
    orr r10, r10, #0xf000
    strh r10, [r9, #4]
    cmp r6, #0x100
    add r3, r3, #8
    add r4, r4, #1
    blt _50
    add r5, r5, #0x40
    cmp r5, #0xc0
    add r12, r12, #8
    blt _3c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    bx lr
    dcd 0x000001ff
    dcd 0xc0000c00
}
