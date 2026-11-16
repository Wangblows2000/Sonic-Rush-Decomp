// asm: C-impossible: switch split point
#include <nitro/types.h>

extern void BossF_Unk_ov000_022f8a48(void);

asm void BossF_State_Unk_ov000_022f8224(void)
{
    stmdb sp!, {lr}
    sub sp, sp, #4
    add r1, r0, #0x200
    ldrh r2, [r1, #0x10]
    add r2, r2, #1
    strh r2, [r1, #0x10]
    ldrh r1, [r1, #0x10]
    and r1, r1, #0xf
    cmp r1, #8
    bgt _6c
    cmp r1, #8
    bge _a4
    cmp r1, #4
    addgt sp, sp, #4
    ldmgtia sp!, {lr}
    bxgt lr
    cmp r1, #0
    addlt sp, sp, #4
    ldmltia sp!, {lr}
    bxlt lr
    cmp r1, #0
    beq _80
    cmp r1, #4
    beq _c8
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
_6c:
    cmp r1, #0xc
    beq _c8
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
_80:
    ldr r0, [r0, #0x20c]
    ands r0, r0, #2
    addeq sp, sp, #4
    ldmeqia sp!, {lr}
    bxeq lr
    bl BossF_Unk_ov000_022f8a48
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
_a4:
    ldr r0, [r0, #0x20c]
    ands r0, r0, #4
    addeq sp, sp, #4
    ldmeqia sp!, {lr}
    bxeq lr
    bl BossF_Unk_ov000_022f8a48
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
_c8:
    ldr r0, [r0, #0x20c]
    ands r0, r0, #8
    addeq sp, sp, #4
    ldmeqia sp!, {lr}
    bxeq lr
    bl BossF_Unk_ov000_022f8a48
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
}
