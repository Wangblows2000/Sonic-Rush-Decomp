// asm: C-impossible: needs two pool words to the same symbol (data_0207cfc8 twice, two symbols at one address)
#include <nitro/types.h>

extern void DC_FlushRange(void);
extern void FSi_GetOverlayBinarySize(void);
extern void MI_CpuFill8(void);
extern void OS_Terminate(void);
extern u32 data_0207cfc8;
extern void func_02000914(void);
extern void func_0200c4c0(void);

asm void FS_StartOverlay(void)
{
    stmdb sp!, {r4, r5, r6, lr}
    mov r5, r0
    bl FSi_GetOverlayBinarySize
    ldr r1, [pc, #0xdc]
    mov r4, r0
    ldrh r0, [r1]
    cmp r0, #2
    bne _94
    ldrb r1, [r5, #0x1f]
    mov r0, #0
    ands r1, r1, #2
    beq _70
    ldr r1, [pc, #0xbc]
    ldr r3, [pc, #0xbc]
    ldr r2, [pc, #0xbc]
    sub r12, r1, r3
    smull r1, lr, r2, r12
    mov lr, lr, asr #3
    mov r1, r12, lsr #0x1f
    ldr r2, [r5]
    add lr, r1, lr
    cmp r2, lr
    bhs _70
    mov r0, #0x14
    mla r0, r2, r0, r3
    ldr r1, [r5, #4]
    mov r2, r4
    bl func_0200c4c0
_70:
    cmp r0, #0
    bne _94
    ldr r0, [r5, #4]
    mov r2, r4
    mov r1, #0
    bl MI_CpuFill8
    bl OS_Terminate
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
_94:
    ldrb r0, [r5, #0x1f]
    ands r0, r0, #1
    beq _ac
    ldr r0, [r5, #4]
    add r0, r0, r4
    bl func_02000914
_ac:
    ldr r0, [r5, #4]
    ldr r1, [r5, #8]
    bl DC_FlushRange
    ldr r6, [r5, #0x10]
    ldr r4, [r5, #0x14]
    cmp r6, r4
    ldmhsia sp!, {r4, r5, r6, lr}
    bxhs lr
_cc:
    ldr r0, [r6]
    cmp r0, #0
    beq _dc
    blx r0
_dc:
    add r6, r6, #4
    cmp r6, r4
    blo _cc
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    dcd 0x027ffc40
    dcd data_0207cfc8
    dcd data_0207cfc8
    dcd 0x66666667
}
