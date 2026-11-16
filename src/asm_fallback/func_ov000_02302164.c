// asm: fallback — C attempts did not match (best C draft: none)
#include <nitro/types.h>

extern void func_02066a9c(void);

asm void func_ov000_02302164(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    sub sp, sp, #8
    ldr r1, [r0, #0x14]
    mov r9, #0
    mov r2, r1, asr #8
    ldr r1, [r0, #0x10]
    sub r2, r2, #3
    ldrb r10, [r0, #0x216]
    mov r0, r2, lsl #0x10
    mov r1, r1, lsl #8
    mov r7, r0, asr #0x10
    mov r4, r1, asr #0x10
    sub r6, r7, r10
    sub r5, r4, r10
    str r9, [sp]
    mov r0, r5
    mov r1, r6
    mov r2, r9
    mov r3, #3
    mov r8, r9
    str r9, [sp, #4]
    bl func_02066a9c
    cmp r0, #0
    ble _88
    mov r12, r9
    str r12, [sp]
    mov r0, r5
    mov r1, r6
    mov r2, #1
    mov r3, #3
    str r12, [sp, #4]
    bl func_02066a9c
    cmp r0, #0
    bgt _90
_88:
    orr r0, r9, #1
    and r9, r0, #0xff
_90:
    add r4, r4, r10
    mov r2, #0
    str r2, [sp]
    mov r0, r4
    mov r1, r6
    mov r3, #3
    str r2, [sp, #4]
    bl func_02066a9c
    cmp r0, #0
    ble _e0
    mov r12, #0
    str r12, [sp]
    mov r0, r4
    mov r1, r6
    mov r2, #1
    mov r3, #3
    str r12, [sp, #4]
    bl func_02066a9c
    cmp r0, #0
    bgt _e8
_e0:
    orr r0, r9, #2
    and r9, r0, #0xff
_e8:
    add r6, r7, r10
    mov r2, #0
    str r2, [sp]
    mov r0, r5
    mov r1, r6
    mov r3, #3
    str r2, [sp, #4]
    bl func_02066a9c
    cmp r0, #0
    ble _138
    mov r7, #0
    str r7, [sp]
    mov r0, r5
    mov r1, r6
    mov r2, #1
    mov r3, #3
    str r7, [sp, #4]
    bl func_02066a9c
    cmp r0, #0
    bgt _140
_138:
    orr r0, r9, #4
    and r9, r0, #0xff
_140:
    mov r2, #0
    str r2, [sp]
    mov r0, r4
    mov r1, r6
    mov r3, #3
    str r2, [sp, #4]
    bl func_02066a9c
    cmp r0, #0
    ble _18c
    mov r5, #0
    str r5, [sp]
    mov r0, r4
    mov r1, r6
    mov r2, #1
    mov r3, #3
    str r5, [sp, #4]
    bl func_02066a9c
    cmp r0, #0
    bgt _194
_18c:
    orr r0, r9, #8
    and r9, r0, #0xff
_194:
    cmp r9, #5
    orreq r0, r8, #1
    andeq r8, r0, #0xff
    cmp r9, #3
    orreq r0, r8, #2
    andeq r8, r0, #0xff
    cmp r9, #0xa
    moveq r0, #1
    movne r0, #0
    ands r0, r0, #0xa
    orrne r0, r8, #4
    andne r8, r0, #0xff
    cmp r9, #0xc
    moveq r0, #1
    movne r0, #0
    ands r0, r0, #0xc
    orrne r0, r8, #8
    andne r8, r0, #0xff
    mov r0, r8
    add sp, sp, #8
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    bx lr
}
