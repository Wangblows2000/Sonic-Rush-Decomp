// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gDisplayTable;
extern u32 gGameState;
extern u32 data_ov000_02368b08;
extern u32 data_ov000_0236aab0;
extern u32 gCamera;
extern u32 data_ov000_0236ac1c;

asm void ZoneBg_Update(void)
{
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    ldr r0, [pc, #0x90]
    ldrh r0, [r0]
    ands r0, r0, #1
    addeq sp, sp, #4
    ldmeqia sp!, {r4, r5, lr}
    bxeq lr
    ldr r1, [pc, #0x7c]
    ldr r2, [pc, #0x7c]
    ldrb r3, [r1, #0x1c]
    mov r0, #0xc
    ldrb r1, [r1, #0x1d]
    mla r0, r3, r0, r2
    ldr r1, [r0, r1, lsl #2]
    cmp r1, #0
    beq _50
    ldr r0, [pc, #0x60]
    ldr r0, [r0]
    blx r1
_50:
    ldr r5, [pc, #0x58]
    mov r4, #0
    ldr lr, [pc, #0x54]
    ldr r1, [pc, #0x54]
    mov r0, #0x64
_64:
    mla r3, r4, r0, r5
    ldr r2, [r3, #0x2c]
    ldr r12, [lr, r4, lsl #2]
    and r2, r1, r2, asr #8
    strh r2, [r12, #4]
    ldr r3, [r3, #0x30]
    add r2, r4, #1
    and r3, r1, r3, asr #8
    and r4, r2, #0xff
    strh r3, [r12, #6]
    cmp r4, #1
    bls _64
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
    dcd data_ov000_0236aab0
    dcd gGameState
    dcd data_ov000_02368b08
    dcd data_ov000_0236ac1c
    dcd gCamera
    dcd gDisplayTable
    dcd 0x000001ff
}
