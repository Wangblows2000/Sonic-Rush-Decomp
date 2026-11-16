// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gCamera;
extern u32 data_ov000_0236ab3c;
extern u32 gPlayers;

asm void StageTitleCard_HoldPlayerY(void)
{
    ldr r1, [r0, #4]
    mov r3, #0
    add r2, r1, #0x400
    strh r3, [r2, #0x20]
    add r2, r1, #0x300
    strh r3, [r2, #0xac]
    ldrsh r3, [r2, #0x58]
    ldrsh r2, [r1, #0x34]
    cmp r3, r2
    strgth r3, [r1, #0x34]
    ldr r2, [r0, #0x488]
    ldr r12, [r1, #0x14]
    ldr r3, [r0, #0x484]
    ands r2, r2, #0x20000000
    ldreq r2, [pc, #0x70]
    sub r3, r12, r3
    str r12, [r1, #0x20]
    andeq r3, r3, r2
    ldr r2, [r0, #0x484]
    ldr r0, [pc, #0x60]
    add r2, r2, r3
    str r2, [r1, #0x14]
    ldr r2, [r0, #0xc8]
    ands r2, r2, #1
    ldrne r2, [r1, #0x14]
    ldrne r1, [r0, #0x40]
    subne r1, r2, r1
    strne r1, [r0, #8]
    bxne lr
    ldrsb r2, [r0, #0x3a]
    ldr r12, [pc, #0x38]
    ldr r1, [r0, #0x40]
    ldr r3, [r12, r2, lsl #2]
    ldr r2, [pc, #0x30]
    ldr r3, [r3, #0x14]
    sub r1, r3, r1
    str r1, [r0, #8]
    ldrsb r1, [r2, #0x3a]
    ldr r0, [r2, #0x40]
    ldr r1, [r12, r1, lsl #2]
    ldr r1, [r1, #0x14]
    sub r0, r1, r0
    str r0, [r2, #8]
    bx lr
    dcd 0x0000ffff
    dcd gCamera
    dcd gPlayers
    dcd data_ov000_0236ab3c
}
