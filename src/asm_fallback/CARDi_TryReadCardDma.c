// asm: fallback - C matches bytes but CW folds the OS_IrqHandler (ITCM) relocation into a raw constant
#include <nitro/types.h>

extern void DC_InvalidateRange(void);
extern void DC_StoreRange(void);
extern void DC_WaitWriteBufferEmpty(void);
extern void IC_InvalidateRange(void);
extern void OS_DisableInterrupts(void);
extern void OS_EnableIrqMask(void);
extern void OS_IrqHandler(void);
extern void OS_ResetRequestIrqMask(void);
extern void OS_RestoreInterrupts(void);
extern u32 data_0207f2c0;
extern void OS_SetIrqFunction(void);
extern void OS_GetDTCMAddress(void);
extern void CARDi_OnReadCard(void);
extern void CARDi_SetCardDma(void);

asm void CARDi_TryReadCardDma(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    ldr r9, [pc, #0x134]
    mov r7, #0
    ldr r4, [r9, #0x1c]
    mov r10, r0
    mov r6, r7
    mov r5, r7
    mov r1, r7
    ands r0, r4, #3
    bne _34
    ldr r0, [r9, #0x24]
    cmp r0, #3
    movls r1, #1
_34:
    cmp r1, #0
    beq _8c
    ldr r8, [r9, #0x20]
    bl OS_GetDTCMAddress
    ldr r1, [pc, #0xf8]
    add r2, r4, r8
    cmp r2, r1
    mov r3, #1
    mov r1, #0
    bls _64
    cmp r4, #0x2000000
    movlo r1, r3
_64:
    cmp r1, #0
    bne _84
    cmp r0, r2
    bhs _80
    add r0, r0, #0x4000
    cmp r0, r4
    bhi _84
_80:
    mov r3, #0
_84:
    cmp r3, #0
    moveq r5, #1
_8c:
    cmp r5, #0
    beq _ac
    ldr r2, [r9, #0x18]
    ldr r1, [r9, #0x20]
    ldr r0, [pc, #0xa4]
    orr r1, r2, r1
    ands r0, r1, r0
    moveq r6, #1
_ac:
    cmp r6, #0
    beq _c0
    ldr r0, [r9, #0x20]
    cmp r0, #0
    movne r7, #1
_c0:
    ldr r0, [pc, #0x84]
    cmp r7, #0
    ldr r0, [r0]
    bic r0, r0, #0x7000000
    orr r0, r0, #0xa1000000
    str r0, [r10, #4]
    beq _134
    bl OS_DisableInterrupts
    mov r4, r0
    ldr r0, [r9, #0x1c]
    ldr r1, [r9, #0x20]
    bl DC_StoreRange
    ldr r0, [r9, #0x1c]
    ldr r1, [r9, #0x20]
    bl DC_InvalidateRange
    ldr r0, [r9, #0x1c]
    ldr r1, [r9, #0x20]
    bl IC_InvalidateRange
    ldr r1, [pc, #0x40]
    mov r0, #0x80000
    bl OS_SetIrqFunction
    mov r0, #0x80000
    bl OS_ResetRequestIrqMask
    mov r0, #0x80000
    bl OS_EnableIrqMask
    bl DC_WaitWriteBufferEmpty
    mov r0, r4
    bl OS_RestoreInterrupts
    bl CARDi_SetCardDma
_134:
    mov r0, r7
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    bx lr
    dcd data_0207f2c0
    dcd OS_IrqHandler
    dcd 0x000001ff
    dcd 0x027ffe60
    dcd CARDi_OnReadCard
}
