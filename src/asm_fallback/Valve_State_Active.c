// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void _s32_div_f(void);
extern u32 gRandSeed;
extern u32 data_ov000_02364474;
extern u32 data_ov000_02364476;
extern u32 data_ov000_02364478;
extern u32 data_ov000_0236447a;
extern void Valve_SpawnSteamPuff(void);
extern void Valve_DrawSprites(void);
extern void Valve_Action_End(void);

asm void Valve_State_Active(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    mov r4, r0
    bl Valve_DrawSprites
    ldr r2, [pc, #0xf0]
    ldr r1, [pc, #0xf0]
    ldr r0, [r2]
    ldr r3, [pc, #0xec]
    mla r5, r0, r1, r3
    mov r0, r5, lsr #0x10
    mov r0, r0, lsl #0x10
    mov r0, r0, lsr #0x10
    str r5, [r2]
    ands r0, r0, #0xf
    bne _d4
    mla r0, r5, r1, r3
    mla r3, r0, r1, r3
    ldr r7, [r4, #0x2a4]
    mov r1, r3, lsr #0x10
    mov r5, r0, lsr #0x10
    mov r9, r1, lsl #0x10
    mov r8, r5, lsl #0x10
    ldr r1, [pc, #0xb4]
    mov r12, r7, lsl #3
    ldr r6, [r4, #0x10]
    ldr r5, [r4, #0x14]
    ldrsh r1, [r1, r12]
    str r0, [r2]
    mov r0, r9, lsr #0x10
    str r3, [r2]
    mov r8, r8, lsr #0x10
    bl _s32_div_f
    ldr r0, [pc, #0x90]
    mov r2, r7, lsl #3
    mov r9, r1
    ldrsh r1, [r0, r2]
    mov r0, r8
    bl _s32_div_f
    mov r2, r1
    mov r8, r7, lsl #3
    mov r0, r4
    mov r1, #0xf1
    ldr r7, [pc, #0x6c]
    ldr r3, [pc, #0x6c]
    ldrsh r7, [r7, r8]
    ldrsh r3, [r3, r8]
    add r6, r7, r6, asr #8
    add r6, r6, r9
    add r3, r3, r5, asr #8
    add r3, r3, r2
    mov r2, r6, lsl #8
    mov r3, r3, lsl #8
    bl Valve_SpawnSteamPuff
_d4:
    ldr r0, [r4, #0xc]
    sub r0, r0, #1
    str r0, [r4, #0xc]
    ldr r0, [r4, #0xc]
    cmp r0, #0
    addne sp, sp, #4
    ldmneia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bxne lr
    mov r0, r4
    bl Valve_Action_End
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bx lr
    dcd gRandSeed
    dcd 0x00196225
    dcd 0x3c6ef35f
    dcd data_ov000_02364478
    dcd data_ov000_0236447a
    dcd data_ov000_02364474
    dcd data_ov000_02364476
}
