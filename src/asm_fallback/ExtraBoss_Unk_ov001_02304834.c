// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gRandSeed;
extern u32 data_ov001_0231f520;
extern u32 data_ov001_0231f54c;
extern void FX_ModS32(void);
extern void ExtraBoss_Unk_ov001_022fc89c(void);
extern void ExtraBoss_Unk_ov001_022fc8bc(void);

asm void ExtraBoss_Unk_ov001_02304834(void)
{
    stmdb sp!, {r4, lr}
    ldr r0, [pc, #0xf4]
    ldr r2, [r0]
    ldr r1, [r2, #0xb3c]
    sub r1, r1, #1
    str r1, [r2, #0xb3c]
    ldr r0, [r0]
    ldr r0, [r0, #0xb3c]
    cmp r0, #0
    ldmgtia sp!, {r4, lr}
    bxgt lr
    ldr r3, [pc, #0xd0]
    ldr r0, [pc, #0xd0]
    ldr r4, [r3]
    ldr r2, [pc, #0xcc]
    mov r1, #6
    mla r2, r4, r0, r2
    mov r0, r2, lsr #0x10
    mov r0, r0, lsl #0x10
    mov r0, r0, lsr #0x10
    str r2, [r3]
    bl FX_ModS32
    ldr r1, [pc, #0xa0]
    ldr r2, [pc, #0xac]
    ldr r3, [r1]
    ldr lr, [r2]
    ldr r2, [r3, #0xb40]
    mov r4, r0, lsl #0x11
    add r0, r2, #1
    str r0, [r3, #0xb40]
    ldr r12, [r1]
    rsb r1, lr, #0
    ldr r0, [r12, #0xb40]
    add r2, r4, #0x10000
    bic r1, r1, #0x1f
    cmp r0, #0xa
    add r2, r2, r1
    blt _b0
    mov r0, #0
    mov r3, r0
    mov r1, #0x108000
    str r0, [r12, #0xb40]
    bl ExtraBoss_Unk_ov001_022fc89c
    b _c0
_b0:
    mov r0, #0
    mov r3, r0
    mov r1, #0x108000
    bl ExtraBoss_Unk_ov001_022fc8bc
_c0:
    ldr r3, [pc, #0x3c]
    ldr r2, [pc, #0x34]
    ldr r12, [r3]
    ldr r0, [pc, #0x34]
    ldr r1, [pc, #0x34]
    ldr r2, [r2]
    mla r1, r12, r0, r1
    mov r0, r1, lsr #0x10
    mov r0, r0, lsl #0x10
    mov r0, r0, lsr #0x10
    and r0, r0, #0x1f
    str r1, [r3]
    add r0, r0, #0x14
    str r0, [r2, #0xb3c]
    ldmia sp!, {r4, lr}
    bx lr
    dcd data_ov001_0231f54c
    dcd gRandSeed
    dcd 0x00196225
    dcd 0x3c6ef35f
    dcd data_ov001_0231f520
}
