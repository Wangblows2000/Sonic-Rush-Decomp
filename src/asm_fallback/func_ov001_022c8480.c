// asm: fallback — C attempts did not match (best C draft: /home/claude/w_ov001j/sw_func_ov001_022c8480.c)
#include <nitro/types.h>

extern void MI_CpuFill8(void);
extern u32 data_022c4560;
extern u32 data_022c4680;
extern u32 data_ov001_02315ebc;
extern void func_0203438c(void);
extern void func_020367b4(void);
extern void func_020372d0(void);
extern void func_02037d60(void);
extern void func_ov001_022c723c(void);
extern void func_ov001_022c79f8(void);
extern void func_ov001_022c7a34(void);
extern void func_ov001_022c7ac8(void);
extern void func_ov001_022c7d2c(void);
extern void func_ov001_022c7d5c(void);
extern void func_ov001_022c7fe0(void);
extern void func_ov001_022c8140(void);
extern void func_ov001_022c8404(void);

asm void func_ov001_022c8480(void)
{
    stmdb sp!, {r4, lr}
    sub sp, sp, #8
    mov r1, #0x4000000
    ldr r0, [r1]
    ldr r2, [pc, #0x178]
    bic r0, r0, #0x1f00
    str r0, [r1]
    ldr r1, [r2]
    ldr r0, [pc, #0x16c]
    bic r1, r1, #0x1f00
    str r1, [r2]
    str r0, [sp]
    mov r2, #0x6b0
    str r2, [sp, #4]
    ldr r0, [pc, #0x158]
    ldr r1, [pc, #0x158]
    mov r2, #0
    mov r3, #0x5000
    bl func_020372d0
    ldr r4, [r0, #0x10]
    mov r1, #0
    mov r0, r4
    mov r2, #0x6b0
    bl MI_CpuFill8
    bl func_02037d60
    ldr r0, [pc, #0x134]
    ldr r0, [r0]
    cmp r0, #0
    moveq r0, #0
    movne r0, #1
    str r0, [r4]
    ldr r0, [r4]
    cmp r0, #0
    beq _94
    cmp r0, #1
    beq _ac
    b _c0
_94:
    ldr r0, [pc, #0x108]
    ldrh r0, [r0, #0xc]
    cmp r0, #0x1a
    movhs r0, #1
    strhs r0, [r4, #0x68c]
    b _c0
_ac:
    ldr r0, [pc, #0xf0]
    ldrh r0, [r0, #0x64]
    cmp r0, #0x1c
    movhs r0, #1
    strhs r0, [r4, #0x68c]
_c0:
    ldr r0, [r4]
    cmp r0, #0
    beq _d8
    cmp r0, #1
    beq _e8
    b _f4
_d8:
    ldr r1, [pc, #0xc8]
    mov r0, r4
    bl func_ov001_022c7a34
    b _f4
_e8:
    ldr r1, [pc, #0xbc]
    mov r0, r4
    bl func_ov001_022c7a34
_f4:
    mov r0, r4
    bl func_ov001_022c8140
    mov r0, r4
    bl func_ov001_022c7fe0
    mov r0, r4
    bl func_ov001_022c7d5c
    mov r0, r4
    bl func_ov001_022c7d2c
    add r0, r4, #0x5c
    bl func_0203438c
    add r0, r4, #0x19c
    bl func_0203438c
    ldr r0, [pc, #0x84]
    add r0, r4, r0
    bl func_0203438c
    ldr r0, [pc, #0x7c]
    add r0, r4, r0
    bl func_0203438c
    ldr r0, [pc, #0x74]
    add r0, r4, r0
    bl func_0203438c
    mov r1, #0x4000000
    ldr r0, [r1]
    ldr r2, [pc, #0x38]
    bic r0, r0, #0x1f00
    orr r0, r0, #0x300
    str r0, [r1]
    ldr r1, [r2]
    mov r0, #0
    bic r1, r1, #0x1f00
    orr r1, r1, #0x1200
    str r1, [r2]
    ldr r2, [r4]
    ldr r1, [pc, #0x3c]
    ldr r1, [r1, r2, lsl #2]
    bl func_020367b4
    add sp, sp, #8
    ldmia sp!, {r4, lr}
    bx lr
    dcd 0x04001000
    dcd 0x00001173
    dcd func_ov001_022c7ac8
    dcd func_ov001_022c8404
    dcd data_022c4560
    dcd data_022c4680
    dcd func_ov001_022c79f8
    dcd func_ov001_022c723c
    dcd 0x0000041c
    dcd 0x0000049c
    dcd 0x000004dc
    dcd data_ov001_02315ebc
}
