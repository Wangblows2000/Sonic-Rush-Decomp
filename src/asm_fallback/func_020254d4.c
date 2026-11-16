// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void FX_GetDivResultFx64c(void);
extern void FX_InvAsync(void);
extern u32 data_02084d40;
extern u32 data_02084d84;
extern void func_02001558(void);
extern void func_0202205c(void);

asm void func_020254d4(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #0x1c
    mov r5, r1
    mov r4, r2
    ldr r6, [pc, #0x178]
    ldr r1, [pc, #0x178]
    add r2, sp, #0x10
    bl func_02001558
    ldr r1, [sp, #0x14]
    ldr r0, [r6, #0x1c]
    ldr r2, [sp, #0x10]
    smull r8, r7, r1, r0
    ldr r0, [r6, #0xc]
    ldr r3, [sp, #0x18]
    smlal r8, r7, r2, r0
    ldr r1, [r6, #0x2c]
    ldr r0, [r6, #0x3c]
    smlal r8, r7, r3, r1
    mov r1, r8, lsr #0xc
    orr r1, r1, r7, lsl #20
    add r0, r1, r0
    bl FX_InvAsync
    ldr r2, [sp, #0x14]
    ldr r1, [r6, #0x10]
    ldr r0, [r6, #0x14]
    smull lr, r8, r2, r1
    smull r12, r9, r2, r0
    ldr r1, [sp, #0x10]
    ldr r3, [r6]
    ldr r0, [sp, #0x18]
    smlal lr, r8, r1, r3
    ldr r7, [r6, #0x20]
    ldr r2, [r6, #0x30]
    smlal lr, r8, r0, r7
    mov r3, lr, lsr #0xc
    orr r3, r3, r8, lsl #20
    add r7, r3, r2
    ldr r3, [r6, #4]
    ldr r8, [r6, #0x24]
    smlal r12, r9, r1, r3
    smlal r12, r9, r0, r8
    mov r0, r12, lsr #0xc
    ldr r2, [r6, #0x34]
    orr r0, r0, r9, lsl #20
    add r6, r0, r2
    bl FX_GetDivResultFx64c
    mov r2, r7, asr #0x1f
    umull r9, lr, r0, r7
    mla lr, r0, r2, lr
    mov r3, #0x80000000
    mla lr, r1, r7, lr
    adds r9, r9, r3
    adc r2, lr, #0
    add r2, r2, #0x1000
    add r2, r2, r2, lsr #31
    mov r7, r2, asr #1
    umull lr, r2, r0, r6
    mov r12, r6, asr #0x1f
    mla r2, r0, r12, r2
    mla r2, r1, r6, r2
    adds r3, lr, r3
    adc r0, r2, #0
    add r0, r0, #0x1000
    add r0, r0, r0, lsr #31
    mov r8, #0
    mov r6, r0, asr #1
    cmp r7, #0
    blt _128
    cmp r6, #0
    blt _128
    cmp r7, #0x1000
    bgt _128
    cmp r6, #0x1000
    ble _12c
_128:
    mvn r8, #0
_12c:
    add r0, sp, #0
    add r1, sp, #4
    add r2, sp, #8
    add r3, sp, #0xc
    bl func_0202205c
    ldr r2, [sp]
    ldr r0, [sp, #8]
    ldr r12, [sp, #0xc]
    sub r0, r0, r2
    mul r0, r7, r0
    add r1, r0, #0x800
    ldr r3, [sp, #4]
    add r1, r2, r1, asr #12
    sub r0, r12, r3
    mul r0, r6, r0
    str r1, [r5]
    ldr r1, [sp, #4]
    add r0, r0, #0x800
    rsb r1, r1, #0xbf
    sub r1, r1, r0, asr #12
    mov r0, r8
    str r1, [r4]
    add sp, sp, #0x1c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bx lr
    dcd data_02084d40
    dcd data_02084d84
}
