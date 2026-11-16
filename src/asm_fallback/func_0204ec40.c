// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_02074bb4;
extern u32 gGameState;
extern u32 data_022c4590;
extern u32 gSaveData;
extern u32 data_022c468c;
extern void func_0204e614(void);

asm void func_0204ec40(void)
{
    stmdb sp!, {r4, lr}
    ldr r0, [pc, #0xc4]
    ldr r4, [pc, #0xc4]
    ldr r0, [r0]
    cmp r0, #1
    beq _4c
    ldrh r0, [r4]
    bl func_0204e614
    ldr r1, [pc, #0xa8]
    strh r0, [r1, #0xe]
    ldrb r2, [r4, #2]
    ldr r1, [pc, #0xa4]
    ldr r0, [pc, #0x98]
    cmp r2, #0x63
    movhi r2, #0x63
    strb r2, [r1, #0x14]
    ldrb r1, [r4, #3]
    strh r1, [r0, #0x28]
    b _ac
_4c:
    ldrh r0, [r4, #0x58]
    bl func_0204e614
    ldr r1, [pc, #0x74]
    mov lr, #0
    strh r0, [r1, #0xe]
    ldrb r2, [r4, #0x5a]
    ldr r0, [pc, #0x64]
    ldr r1, [pc, #0x68]
    cmp r2, #0x63
    movhi r2, #0x63
    strb r2, [r1, #0x14]
    ldr r3, [pc, #0x5c]
    strh lr, [r0, #0x28]
    mov r2, #1
_84:
    ldrh r12, [r4, #0x58]
    ldrb r1, [r3, lr]
    cmp r12, r1
    ldrgeh r1, [r0, #0x28]
    orrge r1, r1, r2, lsl lr
    strgeh r1, [r0, #0x28]
    add r1, lr, #1
    and lr, r1, #0xff
    cmp lr, #7
    blo _84
_ac:
    ldr r0, [pc, #0x2c]
    ldr r0, [r0, #8]
    ands r0, r0, #1
    movne r1, #0
    ldr r0, [pc, #0xc]
    moveq r1, #1
    str r1, [r0, #0x14]
    ldmia sp!, {r4, lr}
    bx lr
    dcd gGameState
    dcd data_022c468c
    dcd data_022c4590
    dcd data_02074bb4
    dcd gSaveData
}
