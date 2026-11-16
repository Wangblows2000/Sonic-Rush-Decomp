// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_0206e470;
extern u32 data_02084d38;
extern u32 data_02084d40;
extern u32 data_02084d84;
extern u32 data_02084f78;
extern u32 data_02084f84;
extern u32 data_02084f90;
extern u32 data_ov001_0231f6a4;
extern u32 data_ov001_0231f6ac;
extern void func_020044f4(void);
extern void func_02004988(void);
extern void func_ov001_02314b28(void);
extern void func_ov001_02314be8(void);

asm void func_ov001_023157d8(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, lr}
    sub sp, sp, #0x10
    ldr r0, [pc, #0xf0]
    ldr r1, [pc, #0xf0]
    ldr r0, [r0, #0x34]
    ldr r2, [r1]
    mov r1, #0x58
    mla r4, r0, r1, r2
    bl func_ov001_02314b28
    ldr r0, [pc, #0xd4]
    ldr r0, [r0, #0x34]
    bl func_ov001_02314be8
    ldrh r2, [r4, #0x48]
    ldr r0, [pc, #0xcc]
    ldr r1, [r4, #0x54]
    and r0, r2, r0
    mov r0, r0, asr #4
    mov r3, r0, lsl #1
    add r0, r3, #1
    str r1, [sp]
    mov r1, #0x1000
    str r1, [sp, #4]
    mov r2, #0
    ldr r1, [pc, #0xa8]
    str r2, [sp, #8]
    str r1, [sp, #0xc]
    ldr r2, [pc, #0xa0]
    mov r3, r3, lsl #1
    mov r1, r0, lsl #1
    ldrsh r0, [r2, r3]
    ldrsh r1, [r2, r1]
    ldr r2, [r4, #0x4c]
    ldr r3, [r4, #0x50]
    bl func_02004988
    ldr r0, [pc, #0x84]
    add r8, r4, #0x20
    ldr r1, [r0, #0xfc]
    ldr r7, [pc, #0x7c]
    bic r1, r1, #0x50
    str r1, [r0, #0xfc]
    add r6, r4, #0x38
    add lr, r4, #0x2c
    ldr r5, [pc, #0x6c]
    ldr r12, [pc, #0x6c]
    ldr r4, [pc, #0x6c]
    mov r3, #0
    ldmia r8, {r0, r1, r2}
    stmia r7, {r0, r1, r2}
    ldmia r6, {r0, r1, r2}
    stmia r5, {r0, r1, r2}
    ldmia lr, {r0, r1, r2}
    stmia r12, {r0, r1, r2}
    str r4, [sp]
    mov r0, r8
    mov r1, r6
    mov r2, lr
    bl func_020044f4
    ldr r0, [pc, #0x28]
    ldr r1, [r0, #0xfc]
    bic r1, r1, #0xe8
    str r1, [r0, #0xfc]
    add sp, sp, #0x10
    ldmia sp!, {r4, r5, r6, r7, r8, lr}
    bx lr
    dcd data_ov001_0231f6ac
    dcd data_ov001_0231f6a4
    dcd 0x0000ffff
    dcd data_02084d40
    dcd data_0206e470
    dcd data_02084d38
    dcd data_02084f78
    dcd data_02084f84
    dcd data_02084f90
    dcd data_02084d84
}
