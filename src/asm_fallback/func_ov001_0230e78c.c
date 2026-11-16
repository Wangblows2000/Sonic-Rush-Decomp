// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void _fadd(void);
extern void _ffix(void);
extern void _fflt(void);
extern void _fmul(void);
extern u32 data_0206e470;
extern void SpecialStage_GetPlayer(void);
extern void func_ov001_0230e634(void);
extern void func_ov001_0230e710(void);

asm void func_ov001_0230e78c(void)
{
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r4, r0
    ldr r0, [r4, #0x24]
    bl _fflt
    ldr r3, [r4, #0xd0]
    ldr r2, [pc, #0x134]
    mov r12, r3, lsl #0x10
    smull r1, lr, r2, r12
    add lr, lr, r3, lsl #16
    mov lr, lr, asr #6
    mov r1, r12, lsr #0x1f
    add lr, r1, lr
    ldr r2, [pc, #0x11c]
    mov r1, lr, lsl #0x10
    and r1, r2, r1, lsr #16
    mov r2, r1, asr #4
    ldr r1, [pc, #0x110]
    mov r2, r2, lsl #2
    mov r5, r0
    ldrsh r0, [r1, r2]
    bl _fflt
    mov r1, r0
    ldr r0, [pc, #0xfc]
    bl _fmul
    mov r1, r0
    mov r0, r5
    bl _fadd
    bl _ffix
    str r0, [r4, #0x24]
    ldr r0, [r4, #0x6c]
    cmp r0, #0x12c
    ble _bc
    mov r2, #0
    str r2, [r4, #0x2c]
    str r2, [r4, #0x30]
    str r2, [r4, #0x34]
    str r2, [r4, #0x38]
    mov r1, #0x200
    str r1, [r4, #0x3c]
    ldr r0, [pc, #0xbc]
    str r1, [r4, #0x40]
    str r0, [r4, #0xcc]
    add sp, sp, #4
    str r2, [r4, #0xd0]
    ldmia sp!, {r4, r5, lr}
    bx lr
_bc:
    ldr r0, [r4, #0xd0]
    cmp r0, #0x78
    addlt sp, sp, #4
    ldmltia sp!, {r4, r5, lr}
    bxlt lr
    bl SpecialStage_GetPlayer
    ldr r3, [r0, #0xe4]
    ldr r1, [r4, #0x20]
    ldr r2, [pc, #0x84]
    sub r3, r3, r1
    smull r1, r12, r2, r3
    mov r1, r3, lsr #0x1f
    mov r12, r12, asr #3
    add r12, r1, r12
    str r12, [r4, #0x2c]
    ldr r12, [r0, #0xe8]
    ldr r1, [r4, #0x24]
    ldr r3, [pc, #0x64]
    sub r12, r12, r1
    smull r1, lr, r2, r12
    mov lr, lr, asr #3
    mov r1, r12, lsr #0x1f
    add lr, r1, lr
    str lr, [r4, #0x30]
    ldr r12, [r0, #0xec]
    ldr r0, [r4, #0x28]
    mov r1, #0
    sub r12, r12, r0
    smull r0, lr, r2, r12
    mov lr, lr, asr #3
    mov r0, r12, lsr #0x1f
    add lr, r0, lr
    str lr, [r4, #0x34]
    str r3, [r4, #0xcc]
    str r1, [r4, #0xd0]
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
    dcd 0x88888889
    dcd 0x0000ffff
    dcd data_0206e470
    dcd 0x3e99999a
    dcd func_ov001_0230e634
    dcd 0x66666667
    dcd func_ov001_0230e710
}
