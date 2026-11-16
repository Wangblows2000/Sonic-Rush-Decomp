// asm: fallback — C attempts did not match (best C draft: /home/claude/w_ov001j/var_func_ov001_022dfc54_nnd.c)
#include <nitro/types.h>

extern void MIi_CpuClear16(void);
extern u32 data_0206e470;
extern u32 data_02084d38;
extern u32 data_02084d40;
extern void func_0200225c(void);
extern void func_02004988(void);

asm void func_ov001_022dfc54(void)
{
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #0x14
    mov r2, #0
    strh r2, [sp, #0x10]
    mov r5, r0
    ldrh r0, [sp, #0x10]
    mov r2, #0x14
    mov r4, r1
    bl MIi_CpuClear16
    ldr r0, [pc, #0x17c]
    mov r2, #0x1000
    strh r0, [r4]
    str r2, [r4, #4]
    mov r1, #0x800000
    ldr r0, [pc, #0x16c]
    str r1, [r4, #8]
    str r0, [r4, #0xc]
    str r2, [r4, #0x10]
    ldrh r2, [r4]
    ldr r1, [r4, #8]
    ldr r0, [pc, #0x158]
    str r1, [sp]
    and r0, r2, r0
    ldr r1, [r4, #0x10]
    mov r0, r0, asr #4
    mov r3, r0, lsl #1
    str r1, [sp, #4]
    mov r2, #0
    add r0, r3, #1
    str r2, [sp, #8]
    ldr r1, [pc, #0x134]
    ldr r2, [pc, #0x134]
    str r1, [sp, #0xc]
    mov r1, r0, lsl #1
    mov r3, r3, lsl #1
    ldrsh r0, [r2, r3]
    ldrsh r1, [r2, r1]
    ldr r2, [r4, #0xc]
    ldr r3, [r4, #4]
    bl func_02004988
    ldr r0, [pc, #0x114]
    mov r2, #0
    ldr r3, [r0, #0xfc]
    mov r1, #0x19000
    bic r3, r3, #0x50
    str r3, [r0, #0xfc]
    str r2, [r4, #0x14]
    str r2, [r4, #0x18]
    str r1, [r4, #0x1c]
    str r2, [r4, #0x2c]
    mov r0, #0x1000
    str r0, [r4, #0x30]
    str r2, [r4, #0x34]
    str r2, [r4, #0x20]
    str r2, [r4, #0x24]
    str r2, [r4, #0x28]
    ldr r0, [r5, #0x10]
    ldr r2, [pc, #0xcc]
    cmp r0, #2
    ldrne r1, [pc, #0xcc]
    ldr r0, [pc, #0xcc]
    ldreq r1, [pc, #0xcc]
    mov r5, r1, lsl #0x10
    add r0, r0, r5, lsr #16
    mov r0, r0, lsl #0x10
    mov r0, r0, lsr #0x10
    mov r0, r0, asr #4
    mov r1, r0, lsl #1
    add r0, r1, #1
    mov r3, r1, lsl #1
    mov r1, r0, lsl #1
    ldrsh r0, [r2, r3]
    ldrsh r1, [r2, r1]
    bl func_0200225c
    ldr r1, [pc, #0x78]
    ldr r2, [pc, #0x7c]
    add r1, r1, r5, lsr #17
    mov r1, r1, lsl #0x10
    and r2, r2, r1, lsr #16
    mov r3, r1, lsr #0x10
    mov r2, r2, asr #4
    mov r1, r2, lsl #1
    add r1, r1, #1
    mov r3, r3, asr #4
    str r0, [r4, #0x38]
    ldr r2, [pc, #0x5c]
    mov r0, r3, lsl #2
    mov r1, r1, lsl #1
    ldrsh r0, [r2, r0]
    ldrsh r1, [r2, r1]
    bl func_0200225c
    mov r1, #0x19000
    mov r3, #0
    umull r12, r5, r0, r1
    mla r5, r0, r3, r5
    mov r0, r0, asr #0x1f
    mov r2, #0x800
    mla r5, r0, r1, r5
    adds r2, r12, r2
    adc r0, r5, #0
    mov r1, r2, lsr #0xc
    orr r1, r1, r0, lsl #20
    str r1, [r4, #0x3c]
    add sp, sp, #0x14
    ldmia sp!, {r4, r5, lr}
    bx lr
    dcd 0x00000888
    dcd 0x00001555
    dcd 0x0000ffff
    dcd data_02084d40
    dcd data_0206e470
    dcd data_02084d38
    dcd 0x0000071c
    dcd 0x00001110
    dcd 0x000004fa
}
