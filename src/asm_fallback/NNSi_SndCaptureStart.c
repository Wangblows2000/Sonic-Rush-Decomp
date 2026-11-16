// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void DC_FlushRange(void);
extern void NNSi_SndFaderInit(void);
extern void _s32_div_f(void);
extern void _u32_div_f(void);
extern u32 data_02083e10;
extern void SND_SetOutputSelector(void);
extern void SND_SetupChannelPcm(void);
extern void SND_SetupAlarm(void);
extern void SND_SetupCapture(void);
extern void SND_StartTimer(void);
extern void NNS_SndFreeAlarm(void);
extern void NNS_SndAllocAlarm(void);
extern void NNS_SndUnlockCapture(void);
extern void NNS_SndLockCapture(void);
extern void NNS_SndLockChannel(void);
extern void AlarmCallback_capture(void);
extern void NNSi_SndFaderSet(void);

asm void NNSi_SndCaptureStart(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x3c
    mov r10, r3
    str r0, [sp, #0x18]
    str r2, [sp, #0x20]
    mov r2, #0
    str r1, [sp, #0x1c]
    mov r0, r1
    mov r1, r10
    ldr r9, [sp, #0x6c]
    ldr r8, [sp, #0x74]
    str r2, [sp, #0x2c]
    mvn r6, #0
    ldr r4, [pc, #0x320]
    bl DC_FlushRange
    ldr r0, [sp, #0x20]
    mov r1, r10
    bl DC_FlushRange
    ldr r0, [sp, #0x60]
    ldr r1, [sp, #0x70]
    cmp r0, #1
    moveq r7, #1
    ldr r0, [pc, #0x300]
    movne r7, #0
    bl _s32_div_f
    ldr r1, [sp, #0x84]
    mov r11, r0
    cmp r1, #0
    beq _bc
    add r1, r11, #0x10
    bic r11, r1, #0x1f
    mov r2, r11, asr #5
    mov r0, r10
    cmp r7, #0
    ldr r1, [sp, #0x80]
    moveq r0, r10, lsr #1
    str r2, [sp, #0x34]
    bl _u32_div_f
    ldr r1, [sp, #0x34]
    mov r5, #0x20
    mul r0, r1, r0
    str r0, [sp, #0x30]
    cmp r7, #0
    ldr r0, [sp, #0x34]
    moveq r5, r5, lsr #1
    mul r0, r5, r0
    mov r5, r0
_bc:
    cmp r7, #0
    movne r0, #0
    strne r0, [sp, #0x28]
    moveq r0, #1
    streq r0, [sp, #0x28]
    cmp r7, #0
    movne r0, #1
    strne r0, [sp, #0x24]
    moveq r0, #0
    streq r0, [sp, #0x24]
    ldr r0, [sp, #0x18]
    cmp r0, #2
    movne r0, #0xa
    strne r0, [sp, #0x2c]
    ldr r0, [sp, #0x84]
    cmp r0, #0
    beq _118
    bl NNS_SndAllocAlarm
    movs r6, r0
    addmi sp, sp, #0x3c
    movmi r0, #0
    ldmmiia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxmi lr
_118:
    mov r0, #3
    bl NNS_SndLockCapture
    cmp r0, #0
    bne _148
    cmp r6, #0
    blt _138
    mov r0, r6
    bl NNS_SndFreeAlarm
_138:
    add sp, sp, #0x3c
    mov r0, #0
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
_148:
    mov r0, #0xa
    bl NNS_SndLockChannel
    cmp r0, #0
    bne _180
    cmp r6, #0
    blt _168
    mov r0, r6
    bl NNS_SndFreeAlarm
_168:
    mov r0, #3
    bl NNS_SndUnlockCapture
    add sp, sp, #0x3c
    mov r0, #0
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
_180:
    mov r0, #0
    mov r7, r10, lsr #2
    str r0, [sp]
    str r7, [sp, #4]
    str r8, [sp, #8]
    str r0, [sp, #0xc]
    cmp r9, #0
    movne r3, #1
    ldr r1, [sp, #0x78]
    str r11, [sp, #0x10]
    str r1, [sp, #0x14]
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x1c]
    moveq r3, #2
    mov r0, #1
    bl SND_SetupChannelPcm
    ldr r1, [sp, #0x64]
    str r9, [sp]
    str r1, [sp, #4]
    ldr r0, [sp, #0x68]
    ldr r1, [sp, #0x24]
    str r0, [sp, #8]
    ldr r2, [sp, #0x1c]
    mov r0, #0
    mov r3, r7
    bl SND_SetupCapture
    mov r0, #0
    str r0, [sp]
    str r7, [sp, #4]
    str r8, [sp, #8]
    str r0, [sp, #0xc]
    cmp r9, #0
    movne r3, #1
    ldr r2, [sp, #0x7c]
    str r11, [sp, #0x10]
    str r2, [sp, #0x14]
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x20]
    moveq r3, #2
    mov r0, #3
    bl SND_SetupChannelPcm
    ldr r2, [sp, #0x64]
    str r9, [sp]
    str r2, [sp, #4]
    ldr r0, [sp, #0x68]
    ldr r1, [sp, #0x24]
    str r0, [sp, #8]
    ldr r2, [sp, #0x20]
    mov r3, r7
    mov r0, #1
    bl SND_SetupCapture
    cmp r6, #0
    blt _270
    ldr r2, [sp, #0x30]
    ldr r3, [pc, #0x104]
    mov r1, r2
    mov r0, r6
    add r1, r1, r5
    str r4, [sp]
    bl SND_SetupAlarm
_270:
    ldr r0, [sp, #0x18]
    cmp r0, #1
    bne _290
    mov r0, #1
    mov r2, r0
    mov r3, r0
    mov r1, #2
    bl SND_SetOutputSelector
_290:
    cmp r6, #0
    movge r0, #1
    movge r2, r0, lsl r6
    ldr r0, [sp, #0x2c]
    movlt r2, #0
    mov r1, #3
    mov r3, #0
    bl SND_StartTimer
    mov r0, #1
    str r0, [r4]
    ldr r0, [sp, #0x18]
    ldr r1, [sp, #0x80]
    str r0, [r4, #4]
    mov r0, #0xa
    str r0, [r4, #0x20]
    ldr r0, [sp, #0x2c]
    str r0, [r4, #0x24]
    mov r0, #3
    str r0, [r4, #0x28]
    ldr r0, [sp, #0x60]
    str r6, [r4, #0x2c]
    str r0, [r4, #8]
    ldr r0, [sp, #0x1c]
    str r0, [r4, #0xc]
    ldr r0, [sp, #0x20]
    str r0, [r4, #0x10]
    mov r0, r10
    str r10, [r4, #0x14]
    bl _u32_div_f
    str r0, [r4, #0x18]
    mov r1, #0
    str r1, [r4, #0x1c]
    ldr r0, [sp, #0x80]
    ldr r1, [sp, #0x84]
    str r0, [r4, #0x30]
    ldr r0, [sp, #0x88]
    str r1, [r4, #0x34]
    str r0, [r4, #0x38]
    add r0, r4, #0x3c
    str r8, [r4, #0x50]
    bl NNSi_SndFaderInit
    add r0, r4, #0x3c
    mov r1, r8, lsl #8
    mov r2, #1
    bl NNSi_SndFaderSet
    mov r0, #0
    str r0, [r4, #0x4c]
    mov r0, #1
    add sp, sp, #0x3c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_02083e10
    dcd 0x00ffb0ff
    dcd AlarmCallback_capture
}
