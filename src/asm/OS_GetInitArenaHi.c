// asm: C-impossible: linker constants in pool with no relocs (raw dcd words)
#include <nitro/types.h>

extern u32 data_0207d2d8;
extern u32 data_027e0000;
extern void OS_GetConsoleType(void);

asm void OS_GetInitArenaHi(void)
{
    stmdb sp!, {lr}
    sub sp, sp, #4
    cmp r0, #6
    addls pc, pc, r0, lsl #2
    b _fc
    b _30
    b _fc
    b _40
    b _80
    b _90
    b _dc
    b _ec
_30:
    add sp, sp, #4
    ldr r0, [pc, #0xd0]
    ldmia sp!, {lr}
    bx lr
_40:
    ldr r0, [pc, #0xc8]
    ldr r0, [r0]
    cmp r0, #0
    beq _60
    bl OS_GetConsoleType
    and r0, r0, #3
    cmp r0, #1
    bne _70
_60:
    add sp, sp, #4
    mov r0, #0
    ldmia sp!, {lr}
    bx lr
_70:
    add sp, sp, #4
    mov r0, #0x2700000
    ldmia sp!, {lr}
    bx lr
_80:
    add sp, sp, #4
    mov r0, #0x2000000
    ldmia sp!, {lr}
    bx lr
_90:
    ldr r0, [pc, #0x7c]
    ldr r1, [pc, #0x7c]
    ldr r2, [pc, #0x7c]
    add r3, r0, #0x3f80
    cmp r1, #0
    sub r2, r3, r2
    bne _c4
    ldr r1, [pc, #0x6c]
    add sp, sp, #4
    cmp r0, r1
    movlo r0, r1
    ldmia sp!, {lr}
    bx lr
_c4:
    cmp r1, #0
    sublt r0, r0, r1
    add sp, sp, #4
    subge r0, r2, r1
    ldmia sp!, {lr}
    bx lr
_dc:
    add sp, sp, #4
    ldr r0, [pc, #0x3c]
    ldmia sp!, {lr}
    bx lr
_ec:
    add sp, sp, #4
    ldr r0, [pc, #0x30]
    ldmia sp!, {lr}
    bx lr
_fc:
    mov r0, #0
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
    dcd 0x023e0000
    dcd data_0207d2d8
    dcd data_027e0000
    dcd 0x00000000
    dcd 0x00000800
    dcd 0x027e2060
    dcd 0x027ff800
    dcd 0x037f8000
}
