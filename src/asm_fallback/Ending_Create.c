// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void MI_CpuFill8(void);
extern u32 gGameState;
extern u32 gSaveData;
extern u32 data_ov001_02315ebc;
extern void Background_Draw(void);
extern void Sound_PlaySeq(void);
extern void Task_Create(void);
extern void Touch_Enable(void);
extern void Ending_SubState_BlazeStart(void);
extern void Ending_SubState_SonicStart(void);
extern void Ending_SetSubState(void);
extern void Ending_Main(void);
extern void Ending_Unk_ov001_022c7d2c(void);
extern void Ending_Unk_ov001_022c7d5c(void);
extern void Ending_LoadAssets(void);
extern void Ending_InitDisplay(void);
extern void Ending_Destructor(void);

asm void Ending_Create(void)
{
    stmdb sp!, {r4, lr}
    sub sp, sp, #8
    mov r1, #0x4000000
    ldr r0, [r1]
    ldr r2, [pc, #0x178]
    bic r0, r0, #0x1f00
    str r0, [r1]
    ldr r1, [r2]
    ldr r0, [pc, #0x16c]
    bic r1, r1, #0x1f00
    str r1, [r2]
    str r0, [sp]
    mov r2, #0x6b0
    str r2, [sp, #4]
    ldr r0, [pc, #0x158]
    ldr r1, [pc, #0x158]
    mov r2, #0
    mov r3, #0x5000
    bl Task_Create
    ldr r4, [r0, #0x10]
    mov r1, #0
    mov r0, r4
    mov r2, #0x6b0
    bl MI_CpuFill8
    bl Touch_Enable
    ldr r0, [pc, #0x134]
    ldr r0, [r0]
    cmp r0, #0
    moveq r0, #0
    movne r0, #1
    str r0, [r4]
    ldr r0, [r4]
    cmp r0, #0
    beq _94
    cmp r0, #1
    beq _ac
    b _c0
_94:
    ldr r0, [pc, #0x108]
    ldrh r0, [r0, #0xc]
    cmp r0, #0x1a
    movhs r0, #1
    strhs r0, [r4, #0x68c]
    b _c0
_ac:
    ldr r0, [pc, #0xf0]
    ldrh r0, [r0, #0x64]
    cmp r0, #0x1c
    movhs r0, #1
    strhs r0, [r4, #0x68c]
_c0:
    ldr r0, [r4]
    cmp r0, #0
    beq _d8
    cmp r0, #1
    beq _e8
    b _f4
_d8:
    ldr r1, [pc, #0xc8]
    mov r0, r4
    bl Ending_SetSubState
    b _f4
_e8:
    ldr r1, [pc, #0xbc]
    mov r0, r4
    bl Ending_SetSubState
_f4:
    mov r0, r4
    bl Ending_InitDisplay
    mov r0, r4
    bl Ending_LoadAssets
    mov r0, r4
    bl Ending_Unk_ov001_022c7d5c
    mov r0, r4
    bl Ending_Unk_ov001_022c7d2c
    add r0, r4, #0x5c
    bl Background_Draw
    add r0, r4, #0x19c
    bl Background_Draw
    ldr r0, [pc, #0x84]
    add r0, r4, r0
    bl Background_Draw
    ldr r0, [pc, #0x7c]
    add r0, r4, r0
    bl Background_Draw
    ldr r0, [pc, #0x74]
    add r0, r4, r0
    bl Background_Draw
    mov r1, #0x4000000
    ldr r0, [r1]
    ldr r2, [pc, #0x38]
    bic r0, r0, #0x1f00
    orr r0, r0, #0x300
    str r0, [r1]
    ldr r1, [r2]
    mov r0, #0
    bic r1, r1, #0x1f00
    orr r1, r1, #0x1200
    str r1, [r2]
    ldr r2, [r4]
    ldr r1, [pc, #0x3c]
    ldr r1, [r1, r2, lsl #2]
    bl Sound_PlaySeq
    add sp, sp, #8
    ldmia sp!, {r4, lr}
    bx lr
    dcd 0x04001000
    dcd 0x00001173
    dcd Ending_Main
    dcd Ending_Destructor
    dcd gGameState
    dcd gSaveData
    dcd Ending_SubState_SonicStart
    dcd Ending_SubState_BlazeStart
    dcd 0x0000041c
    dcd 0x0000049c
    dcd 0x000004dc
    dcd data_ov001_02315ebc
}
