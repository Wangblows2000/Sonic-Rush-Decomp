// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void MapCollision_ProbeDistance(void);

asm void Mine_State_Main(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0xc
    mov r10, r0
    ldr r1, [r10, #0xc]
    mov r0, #0
    add r1, r1, #1
    str r1, [r10, #0xc]
    ldr r1, [r10, #0xc]
    ldr r11, [pc, #0x13c]
    mov r1, r1, asr #4
    sub r1, r1, #1
    and r1, r1, #7
    mov r1, r1, lsl #0x10
    mov r1, r1, asr #0xf
    mov r1, r11, asr r1
    and r1, r1, #3
    mov r9, r0
    strb r1, [r10, #6]
    mov r4, r0
    mov r7, r0
    mov r8, r0
    mov r6, #1
    mov r5, #2
_5c:
    ldr r2, [r10, #0x10]
    add r1, r10, r9, lsl #3
    str r2, [r1, #0x330]
    ldr r2, [r10, #0xc]
    ldr r3, [r1, #0x330]
    add r2, r9, r2, asr #4
    and r2, r2, #7
    mov r2, r2, lsl #0x10
    mov r2, r2, asr #0xf
    mov r2, r11, asr r2
    and r2, r2, #3
    mov r2, r2, lsl #0x10
    add r2, r3, r2, asr #8
    str r2, [r1, #0x330]
    cmp r9, #3
    subeq r2, r9, #1
    addeq r2, r10, r2, lsl #3
    ldreq r2, [r2, #0x334]
    streq r2, [r1, #0x334]
    beq _134
    cmp r0, #0
    strlt r8, [r1, #0x330]
    blt _134
    cmp r9, #4
    bne _d8
    sub r0, r9, #1
    add r0, r10, r0, lsl #3
    ldr r0, [r0, #0x334]
    add r0, r0, #0x1e00
    str r0, [r1, #0x334]
    b _100
_d8:
    cmp r9, #0
    ldreq r0, [r10, #0x14]
    addeq r0, r0, #0x1800
    streq r0, [r1, #0x334]
    beq _100
    sub r0, r9, #1
    add r0, r10, r0, lsl #3
    ldr r0, [r0, #0x334]
    add r0, r0, #0xa00
    str r0, [r1, #0x334]
_100:
    str r7, [sp]
    str r7, [sp, #4]
    add r1, r10, r9, lsl #3
    ldr r0, [r1, #0x330]
    ldr r1, [r1, #0x334]
    mov r2, r6
    mov r3, r5
    mov r0, r0, asr #8
    mov r1, r1, asr #8
    bl MapCollision_ProbeDistance
    cmp r0, #0
    addlt r1, r10, r9, lsl #3
    strlt r4, [r1, #0x330]
_134:
    add r1, r9, #1
    mov r1, r1, lsl #0x10
    mov r9, r1, asr #0x10
    cmp r9, #0x10
    blt _5c
    ldr r0, [r10, #0x54]
    ands r1, r0, #2
    orrne r0, r0, #4
    strne r0, [r10, #0x54]
    add sp, sp, #0xc
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd 0x00009515
}
