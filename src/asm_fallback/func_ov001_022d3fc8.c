// asm: fallback — C attempts did not match (best C draft: /home/claude/w_ov001j/var_func_ov001_022d3fc8_sw3.c)
#include <nitro/types.h>

extern void func_ov001_022d3e4c(void);
extern void func_ov001_022d3eb0(void);

#pragma thumb on
asm void func_ov001_022d3fc8(void)
{
    push {r4, r5, r6, r7, lr}
    sub sp, #0x14
    str r0, [sp, #8]
    ldr r1, [pc, #0x6c]
    ldr r1, [r0, r1]
    add r0, #0xc
    mov r2, #0
    str r2, [sp]
    str r2, [sp, #4]
    add r3, r2, #0
    bl func_ov001_022d3eb0
    mov r2, #0
    str r2, [sp]
    mov r0, #4
    str r0, [sp, #4]
    ldr r0, [sp, #8]
    add r0, #0x54
    ldr r3, [pc, #0x50]
    ldr r1, [sp, #8]
    ldr r1, [r1, r3]
    add r3, r2, #0
    bl func_ov001_022d3e4c
    ldr r1, [pc, #0x40]
    ldr r0, [sp, #8]
    ldr r0, [r0, r1]
    str r0, [sp, #0xc]
    mov r7, #0
    add r5, r7, #0
    ldr r4, [sp, #8]
    add r6, r7, #0
_40:
    str r6, [sp]
    mov r0, #8
    str r0, [sp, #4]
    ldr r1, [sp, #8]
    add r1, #0xc
    add r0, r5, #0
    add r0, #0x90
    add r0, r1, r0
    ldr r1, [sp, #0xc]
    ldr r2, [pc, #0x28]
    ldr r2, [r4, r2]
    lsl r2, r2, #0x10
    lsr r2, r2, #0x10
    add r3, r6, #0
    bl func_ov001_022d3e4c
    add r5, #0x48
    add r4, r4, #4
    add r7, r7, #1
    cmp r7, #4
    blt _40
    add sp, #0x14
    pop {r4, r5, r6, r7}
    pop {r3}
    bx r3
    mov r8, r8
    dcd 0x000005e4
    dcd 0x000005e0
    dcd 0x000005f4
}
#pragma thumb reset
