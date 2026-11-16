// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_02072cb8;
extern u32 data_02075b6c;
extern u32 data_020763e4;
extern u32 data_0207c460;
extern u32 data_022c4678;
extern void func_0203879c(void);

#pragma thumb on
asm void func_02056fa4(void)
{
    push {r4, lr}
    add r3, r0, #0
    ldr r2, [pc, #0x54]
    ldr r1, [pc, #0x58]
    mov r0, #0
    ldrsh r1, [r1, r0]
    lsl r1, r1, #3
    add r2, r2, r1
    ldr r1, [r2, #4]
    cmp r3, r1
    bhs _54
    ldr r0, [pc, #0x4c]
    ldrb r1, [r0]
    cmp r1, #5
    bhi _36
    add r0, r1, r1
    add r0, pc
    ldrh r0, [r0, #4]
    add r0, pc
    bx r0
    lsl r5, r1, #0
    lsl r5, r1, #0
    lsl r5, r1, #0
    lsl r5, r1, #0
    lsl r5, r1, #0
    lsl r5, r1, #0
    b _38
_36:
    mov r1, #1
_38:
    ldr r0, [pc, #0x2c]
    ldr r4, [r2]
    lsl r2, r3, #2
    ldr r3, [r4, r2]
    lsl r2, r1, #2
    ldr r1, [pc, #0x28]
    ldr r1, [r1, r2]
    add r1, r3, r1
    lsl r1, r1, #0x10
    lsr r1, r1, #0x10
    mov r2, #0
    mvn r2, r2
    bl func_0203879c
_54:
    pop {r4}
    pop {r3}
    bx r3
    mov r8, r8
    dcd data_020763e4
    dcd data_022c4678
    dcd data_02072cb8
    dcd data_0207c460
    dcd data_02075b6c
}
#pragma thumb reset
