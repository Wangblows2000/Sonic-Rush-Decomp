// asm: fallback — C attempts did not match (best C draft: /home/claude/w_ov001j/sw_func_ov001_0230e94c.c)
#include <nitro/types.h>

extern void func_ov001_0230d6cc(void);
extern void func_ov001_0230e8fc(void);

asm void func_ov001_0230e94c(void)
{
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r4, r0
    bl func_ov001_0230d6cc
    ldr r1, [r4, #0x78]
    cmp r0, r1
    addne sp, sp, #4
    ldmneia sp!, {r4, r5, lr}
    bxne lr
    ldr r0, [r4, #0x28]
    cmp r0, #0
    addle sp, sp, #4
    ldmleia sp!, {r4, r5, lr}
    bxle lr
    ldrh r0, [r4, #0x60]
    ldr r12, [pc, #0x8c]
    mov r2, #0xa000
    orr r0, r0, #0x20
    strh r0, [r4, #0x60]
    mov r0, #0
    str r0, [r4, #0x78]
    ldr r1, [r4, #0x20]
    mov r3, #0x32000
    rsb lr, r1, #0
    smull r1, r5, r12, lr
    mov r5, r5, asr #3
    mov r1, lr, lsr #0x1f
    add r5, r1, r5
    str r5, [r4, #0x2c]
    ldr lr, [r4, #0x24]
    rsb r1, r2, #0
    sub r2, r1, lr
    smull r1, lr, r12, r2
    mov lr, lr, asr #3
    mov r1, r2, lsr #0x1f
    add lr, r1, lr
    str lr, [r4, #0x30]
    ldr r2, [r4, #0x28]
    rsb r1, r3, #0
    sub r2, r1, r2
    smull r1, r3, r12, r2
    mov r3, r3, asr #3
    mov r1, r2, lsr #0x1f
    add r3, r1, r3
    ldr r1, [pc, #0x1c]
    str r3, [r4, #0x34]
    str r1, [r4, #0xcc]
    str r0, [r4, #0xd0]
    str r0, [r4, #0x6c]
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
    dcd 0x66666667
    dcd func_ov001_0230e8fc
}
