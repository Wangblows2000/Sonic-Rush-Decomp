// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gBossStatus;
extern u32 gBoss4;
extern u32 data_ov000_02369b30;
extern u32 data_ov000_0236a444;
extern u32 data_ov000_0236a524;
extern u32 gPlayers;
extern void Model_SetResource(void);
extern void Model_Init(void);
extern void NaviMsg_ShowMessage(void);
extern void NaviMsg_SetFaceAnim(void);
extern void Boss4_StartAuxModelCycle(void);
extern void Boss4_Action_StartSpin(void);
extern void Boss4_ResetCamera(void);
extern void Boss4_Draw_PlayerOnArena(void);
extern void Boss4_State_Fight(void);
extern void MapObject_Spawn(void);

asm void Boss4_State_WaitForStart(void)
{
    stmdb sp!, {r4, r5, r6, lr}
    sub sp, sp, #0x18
    ldr r1, [pc, #0x130]
    mov r6, r0
    ldr r0, [r1]
    ands r0, r0, #1
    addeq sp, sp, #0x18
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
    ldr r0, [pc, #0x118]
    ldr r1, [pc, #0x118]
    ldr r4, [r0]
    ldr r12, [pc, #0x114]
    ldrh r0, [r4, #0x5c]
    ldr r2, [pc, #0x110]
    ldr r5, [pc, #0x110]
    bic r0, r0, #0x20
    strh r0, [r4, #0x5c]
    ldr r3, [r1]
    mov r0, r5
    str r12, [r3, #0x70]
    ldr r12, [r1]
    mov r1, #0
    ldrh r3, [r12, #0x5c]
    orr r3, r3, #0x2040
    strh r3, [r12, #0x5c]
    ldr r3, [r2]
    orr r3, r3, #6
    str r3, [r2]
    orr r3, r3, #1
    str r3, [r2]
    bl Model_Init
    ldr r1, [pc, #0xd0]
    mov r0, r5
    ldr r1, [r1, #0x18]
    mov r2, #0
    bl Model_SetResource
    ldr r0, [pc, #0xc0]
    str r0, [r5, #0x20]
    ldr r0, [r5, #0x20]
    str r0, [r5, #0x1c]
    ldr r0, [r5, #0x1c]
    str r0, [r5, #0x18]
    bl Boss4_ResetCamera
    mov r1, #0
    str r1, [sp]
    str r1, [sp, #4]
    str r1, [sp, #8]
    str r1, [sp, #0xc]
    str r1, [sp, #0x10]
    mov r0, #0xce
    mov r2, r1
    mov r3, r1
    bl MapObject_Spawn
    mov r1, #0
    str r0, [r4, #0x20c]
    str r1, [sp]
    str r1, [sp, #4]
    str r1, [sp, #8]
    str r1, [sp, #0xc]
    str r1, [sp, #0x10]
    mov r0, #0xcf
    mov r2, r1
    mov r3, r1
    bl MapObject_Spawn
    str r0, [r4, #0x210]
    mov r0, r4
    bl Boss4_StartAuxModelCycle
    mov r0, #9
    bl NaviMsg_SetFaceAnim
    mov r0, #0xf
    mov r1, #0xf0
    bl NaviMsg_ShowMessage
    ldr r1, [pc, #0x34]
    mov r0, r4
    str r1, [r6, #0x68]
    bl Boss4_Action_StartSpin
    add sp, sp, #0x18
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    dcd gBossStatus
    dcd gBoss4
    dcd gPlayers
    dcd Boss4_Draw_PlayerOnArena
    dcd data_ov000_0236a444
    dcd data_ov000_0236a524
    dcd data_ov000_02369b30
    dcd 0x01355b50
    dcd Boss4_State_Fight
}
