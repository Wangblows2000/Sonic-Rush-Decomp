// asm: fallback — C attempts did not match (best C draft: /home/claude/w_ov001j/sw_func_ov001_02303494.c)
#include <nitro/types.h>

extern void func_ov001_02300c94(void);

asm void func_ov001_02303494(void)
{
    stmdb sp!, {r4, r5, r6, lr}
    mov r5, r0
    bl func_ov001_02300c94
    add r4, r0, #0xb0
    ldr r1, [r5]
    ldr r5, [r5, #4]
    ldr r3, [r4, #0xc]
    ldr r2, [r0, #0x18]
    ldr r12, [r4, #0x18]
    add lr, r2, r3
    ldr r3, [r4, #0x10]
    ldr r2, [r0, #0x1c]
    ldr r0, [r4, #0x1c]
    add r2, r2, r3
    sub r6, lr, r12
    sub r3, r1, #0x8000
    cmp r6, r3
    add r4, lr, r12
    sub r12, r2, r0
    add r2, r2, r0
    add r0, r1, #0x8000
    sub r1, r5, #0x10000
    ble _6c
    cmp r6, r0
    movgt r0, #0
    ldmgtia sp!, {r4, r5, r6, lr}
    bxgt lr
_6c:
    cmp r3, r6
    ble _84
    cmp r3, r4
    movgt r0, #0
    ldmgtia sp!, {r4, r5, r6, lr}
    bxgt lr
_84:
    cmp r12, r1
    ble _9c
    cmp r12, r5
    movgt r0, #0
    ldmgtia sp!, {r4, r5, r6, lr}
    bxgt lr
_9c:
    cmp r1, r12
    ble _b4
    cmp r1, r2
    movgt r0, #0
    ldmgtia sp!, {r4, r5, r6, lr}
    bxgt lr
_b4:
    mov r0, #1
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
}
