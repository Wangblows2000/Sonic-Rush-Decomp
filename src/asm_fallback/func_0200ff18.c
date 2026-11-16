// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void CARDi_SetRomOp(void);
extern u32 data_0207f2c0;
extern void func_02010358(void);

asm void func_0200ff18(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    mov r10, r0
    ldr r9, [pc, #0xe0]
    add r7, r10, #0x20
    ldr r5, [pc, #0xdc]
    ldr r6, [pc, #0xdc]
    mov r11, #0
    mov r0, #0x200
    rsb r4, r0, #0
_28:
    ldr r0, [r9, #0x18]
    and r1, r0, r4
    cmp r1, r0
    bne _50
    ldr r8, [r9, #0x1c]
    ands r0, r8, #3
    bne _50
    ldr r0, [r9, #0x20]
    cmp r0, #0x200
    bhs _58
_50:
    mov r8, r7
    str r1, [r10, #8]
_58:
    mov r0, r1, lsr #8
    orr r0, r0, #0xb7000000
    mov r1, r1, lsl #0x18
    bl CARDi_SetRomOp
    ldr r1, [r10, #4]
    mov r0, r11
    str r1, [r6]
_74:
    ldr r2, [r6]
    ands r1, r2, #0x800000
    beq _90
    ldr r1, [r5]
    cmp r0, #0x200
    strlo r1, [r8, r0, lsl #2]
    addlo r0, r0, #1
_90:
    ands r1, r2, #0x80000000
    bne _74
    ldr r0, [r9, #0x1c]
    cmp r8, r0
    bne _d8
    ldr r2, [r9, #0x18]
    ldr r1, [r9, #0x1c]
    ldr r0, [r9, #0x20]
    add r2, r2, #0x200
    add r1, r1, #0x200
    subs r0, r0, #0x200
    str r2, [r9, #0x18]
    str r1, [r9, #0x1c]
    str r0, [r9, #0x20]
    bne _28
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
_d8:
    mov r0, r10
    bl func_02010358
    cmp r0, #0
    bne _28
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_0207f2c0
    dcd 0x04100010
    dcd 0x040001a4
}
