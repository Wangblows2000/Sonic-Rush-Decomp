// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void GX_DispOn(void);
extern void OS_DisableInterrupts(void);
extern void OS_Halt(void);
extern void OS_RestoreInterrupts(void);
extern void OS_SetIrqMask(void);
extern void OS_SpinWait(void);
extern u32 data_0207f090;
extern u32 data_0207f098;
extern void func_02002b70(void);
extern void func_02005348(void);
extern void func_0200de94(void);
extern void func_0200df6c(void);
extern void func_0200dfa8(void);
extern void func_0200e300(void);
extern void func_0200e390(void);
extern void func_0200e3e4(void);
extern void func_0200e4b0(void);
extern void func_0200e738(void);
extern void func_0201ab64(void);

asm void func_0200e098(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x1c
    ldr r3, [pc, #0x204]
    mov r10, r0
    ldr r0, [r3]
    mov r9, r1
    mov r11, r2
    mov r8, #0
    bl func_0200de94
    ldr r1, [pc, #0x1ec]
    mov r0, r8
    ldrh r4, [r1]
    strh r0, [r1]
    bl OS_DisableInterrupts
    str r0, [sp]
    ldr r0, [pc, #0x1d8]
    bl func_02005348
    str r0, [sp, #4]
    mov r0, #0x40000
    bl OS_SetIrqMask
    ldr r0, [sp]
    bl OS_RestoreInterrupts
    ldr r2, [pc, #0x1b8]
    mov r1, #1
    ldrh r0, [r2]
    ands r0, r10, #8
    strh r1, [r2]
    beq _80
    ldr r0, [pc, #0x1a8]
    ldrh r0, [r0]
    cmp r0, #2
    biceq r10, r10, #8
_80:
    ands r0, r10, #0x10
    beq _94
    bl func_0201ab64
    cmp r0, #0
    biceq r10, r10, #0x10
_94:
    ldr r0, [pc, #0x188]
    mov r1, #0x4000000
    ldr r7, [r1]
    ldr r6, [r0]
    bl func_0200df6c
    str r0, [sp, #8]
    add r0, sp, #0xc
    add r1, sp, #0x10
    bl func_0200e300
    mov r0, #2
    mov r1, #0
    bl func_0200e3e4
    ldr r2, [pc, #0x15c]
    ldr r0, [r2]
    str r0, [sp, #0x14]
_d0:
    ldr r1, [r2]
    ldr r0, [sp, #0x14]
    cmp r0, r1
    beq _d0
    ldr r0, [r2]
    str r0, [sp, #0x14]
    bl func_02002b70
    ldr r1, [pc, #0x130]
    ldr r0, [r1]
    bic r0, r0, #0x10000
    str r0, [r1]
    ldr r2, [pc, #0x124]
_100:
    ldr r1, [r2]
    ldr r0, [sp, #0x14]
    cmp r0, r1
    beq _100
    ldr r0, [r2]
    str r0, [sp, #0x14]
    ldr r2, [pc, #0x108]
_11c:
    ldr r1, [r2]
    ldr r0, [sp, #0x14]
    cmp r0, r1
    beq _11c
    ldr r0, [sp, #0xc]
    ldr r1, [sp, #0x10]
    orr r0, r10, r0, lsl #5
    orr r0, r0, r1, lsl #6
    mov r0, r0, lsl #0x10
    mov r5, r0, lsr #0x10
    orr r0, r9, r11
    mov r0, r0, lsl #0x10
    mov r9, r0, lsr #0x10
_150:
    mov r0, r5
    mov r1, r9
    bl func_0200e738
    cmp r0, #0
    bne _150
    bl OS_Halt
    ldr r0, [pc, #0xbc]
    bl OS_SpinWait
    ands r0, r10, #8
    beq _188
    ldr r0, [pc, #0xb0]
    ldr r0, [r0]
    ands r0, r0, #0x100000
    movne r8, #1
_188:
    cmp r8, #0
    bne _1d0
    ldr r0, [sp, #8]
    cmp r0, #1
    bne _1b8
    mov r0, #1
    mov r1, r0
    mov r2, r0
    mov r3, #0
    bl func_0200dfa8
    bl GX_DispOn
    b _1c0
_1b8:
    mov r0, #1
    bl func_0200e4b0
_1c0:
    mov r1, #0x4000000
    ldr r0, [pc, #0x58]
    str r7, [r1]
    str r6, [r0]
_1d0:
    bl OS_DisableInterrupts
    ldr r0, [sp, #4]
    bl OS_SetIrqMask
    ldr r0, [sp]
    bl OS_RestoreInterrupts
    ldr r1, [pc, #0x2c]
    cmp r8, #0
    ldrh r0, [r1]
    strh r4, [r1]
    beq _1fc
    bl func_0200e390
_1fc:
    ldr r0, [pc, #0x30]
    ldr r0, [r0]
    bl func_0200de94
    add sp, sp, #0x1c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_0207f090
    dcd 0x04000208
    dcd 0x003fffff
    dcd 0x027ffc40
    dcd 0x04001000
    dcd 0x027ffc3c
    dcd 0x00332340
    dcd 0x04000214
    dcd data_0207f098
}
