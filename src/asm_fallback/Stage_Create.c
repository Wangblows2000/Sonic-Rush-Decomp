// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void MIi_CpuClear16(void);
extern u32 data_020748bc;
extern u32 data_020748be;
extern u32 data_020748c0;
extern u32 data_020748c2;
extern u32 data_020748c4;
extern u32 data_020748c6;
extern u32 data_020748c8;
extern u32 data_020748ca;
extern u32 data_020748cc;
extern u32 data_02074960;
extern u32 data_02074962;
extern u32 data_02074964;
extern u32 data_02074a04;
extern u32 data_0207cf78;
extern u32 gGameState;
extern u32 data_022c4590;
extern u32 data_022c4ee8;
extern u32 gObjManagerFlags;
extern u32 data_022c4ef4;
extern u32 data_022c4ef8;
extern u32 data_022c4efc;
extern u32 gBossStatus;
extern u32 data_ov000_0236a448;
extern u32 gCamera;
extern void G3X_SetEdgeColorTable(void);
extern void NNS_G3dGlbLightColor(void);
extern void Task_Create(void);
extern void WH_GetCurrentAid(void);
extern void BrightnessFade_Create(void);
extern void ScreenShake_Create(void);
extern void NaviB_Create(void);
extern void Stage_Destructor(void);
extern void Stage_Main(void);
extern void StageNetEvent_Main(void);
extern void Stage_InitMapCollision(void);
extern void Stage_InitBlend(void);
extern void NaviMsg_SetVisible(void);
extern void NaviMsg_Create(void);
extern void StageTitleCard_Create(void);
extern void ObjCollisionMap_CommitFrame(void);
extern void NetPacket_FindSlot(void);
extern void ObjDraw_SetManagedRows(void);
extern void ObjDraw_InitRows(void);
extern void ObjRect_CheckInit(void);
extern void BossCamera_Create(void);
extern void StageZoneScript_Create(void);
extern void EffTrickSparkle_Create(void);
extern void WirelessIcon_Create(void);
extern void FixVsHud_Create(void);
extern void StageEvent_SpawnAllMapObjects(void);
extern void StageEvent_CreateManager(void);
extern void Hud_SetFlags(void);
extern void Hud_GetFlags(void);
extern void Hud_Create(void);
extern void Camera_Create(void);
extern void Player_Create(void);
extern void RingManager_Create(void);

asm void Stage_Create(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #0xc
    mov r1, #3
    mov r2, #0x10
    str r1, [sp]
    mov r4, #4
    ldr r0, [pc, #0x574]
    mov r3, r2
    mov r1, #0
    str r4, [sp, #4]
    bl Task_Create
    mov r2, #3
    str r2, [sp]
    ldr r0, [pc, #0x55c]
    ldr r1, [pc, #0x55c]
    mov r2, #0
    mov r3, #0x4000
    str r4, [sp, #4]
    bl Task_Create
    mov r1, #0
    strh r1, [sp, #8]
    mov r1, r0
    ldrh r0, [sp, #8]
    ldr r1, [r1, #0x10]
    mov r2, r4
    bl MIi_CpuClear16
    ldr r1, [pc, #0x530]
    ldr r3, [pc, #0x530]
    ldrb r4, [r1, #0x1c]
    ldrb r2, [r1, #0x1d]
    mov r0, #0x12
    mov r1, #6
    mla r0, r4, r0, r3
    mul r1, r2, r1
    ldrh r1, [r1, r0]
    mov r0, #0
    bl NNS_G3dGlbLightColor
    ldr r0, [pc, #0x504]
    ldr r2, [pc, #0x508]
    ldrb r4, [r0, #0x1c]
    ldrb r3, [r0, #0x1d]
    mov r1, #0x12
    mla r2, r4, r1, r2
    mov r1, #6
    mul r1, r3, r1
    ldrh r1, [r1, r2]
    mov r0, #1
    bl NNS_G3dGlbLightColor
    ldr r1, [pc, #0x4d8]
    ldr r4, [pc, #0x4e0]
    ldrb r5, [r1, #0x1c]
    ldrb r3, [r1, #0x1d]
    mov r1, #0x12
    mov r2, #6
    mla r1, r5, r1, r4
    mul r2, r3, r2
    ldrh r1, [r2, r1]
    mov r0, #2
    bl NNS_G3dGlbLightColor
    ldr r0, [pc, #0x4ac]
    mov r1, #0
    ldrb r6, [r0, #0x1c]
    cmp r6, #9
    bhs _10c
    ldrb r0, [r0, #0x1d]
    cmp r0, #2
    movlo r1, #1
_10c:
    cmp r1, #0
    bne _190
    mov r0, #0x12
    mul r5, r6, r0
    ldr r4, [pc, #0x48c]
    ldr lr, [pc, #0x48c]
    ldr r12, [pc, #0x48c]
    ldr r3, [pc, #0x48c]
    ldr r2, [pc, #0x48c]
    ldr r1, [pc, #0x48c]
    ldr r0, [pc, #0x48c]
    ldr r9, [pc, #0x48c]
    ldr r8, [pc, #0x48c]
    ldrsh r4, [r4, r5]
    ldr r7, [pc, #0x488]
    ldrsh lr, [lr, r5]
    ldrsh r12, [r12, r5]
    ldrsh r3, [r3, r5]
    ldrsh r2, [r2, r5]
    ldrsh r1, [r1, r5]
    ldrsh r0, [r0, r5]
    ldrsh r9, [r9, r5]
    ldrsh r5, [r8, r5]
    strh r4, [r7]
    strh lr, [r7, #2]
    strh r12, [r7, #4]
    strh r3, [r7, #6]
    strh r2, [r7, #8]
    strh r1, [r7, #0xa]
    strh r0, [r7, #0xc]
    strh r9, [r7, #0xe]
    strh r5, [r7, #0x10]
    b _1d4
_190:
    mov r12, #0xb50
    ldr r0, [pc, #0x438]
    mov r5, #0
    ldr r4, [pc, #0x434]
    ldr r3, [pc, #0x434]
    ldr r2, [pc, #0x434]
    rsb r7, r12, #0
    mov r1, #0x620
    strh r12, [r0]
    strh r7, [r0, #2]
    strh r5, [r0, #4]
    strh r5, [r0, #6]
    strh r4, [r0, #8]
    strh r3, [r0, #0xa]
    strh r2, [r0, #0xc]
    strh r1, [r0, #0xe]
    strh r5, [r0, #0x10]
_1d4:
    ldr r1, [pc, #0x408]
    mov r0, #0x30
    mla r3, r6, r0, r1
    ldr r0, [pc, #0x3b8]
    ldr r6, [pc, #0x3fc]
    ldrb r0, [r0, #0x1d]
    ldr r2, [pc, #0x3e0]
    mov r5, #0
    ldr r1, [pc, #0x3f0]
    mov r4, #3
    add r0, r3, r0, lsl #4
    strh r6, [r2, #0x12]
    strh r5, [r2, #0x14]
    strh r5, [r2, #0x16]
    strh r4, [r1]
    bl G3X_SetEdgeColorTable
    bl ObjDraw_InitRows
    mov r0, r4
    mov r1, #0x10
    bl ObjDraw_SetManagedRows
    bl StageEvent_CreateManager
    bl StageEvent_SpawnAllMapObjects
    bl Camera_Create
    bl Stage_InitMapCollision
    ldr r1, [pc, #0x364]
    ldr r2, [pc, #0x3b0]
    mov r0, r5
    strh r0, [r2, #2]
    ldrsh r5, [r2, #2]
    ldrb r4, [r1, #0x1c]
    ldr r3, [pc, #0x3a0]
    strh r5, [r2]
    cmp r4, #9
    ldr r2, [pc, #0x398]
    mov r4, #0x80
    strh r4, [r2]
    str r0, [r3]
    orr r2, r0, #0x1c
    str r2, [r3]
    bhs _280
    ldrb r1, [r1, #0x1d]
    cmp r1, #2
    blo _29c
_280:
    ldr r0, [pc, #0x36c]
    ldr r1, [r0]
    orr r1, r1, #0x80
    str r1, [r0]
    bic r1, r1, #4
    str r1, [r0]
    b _2a0
_29c:
    bl EffTrickSparkle_Create
_2a0:
    ldr r0, [pc, #0x2f8]
    ldr r2, [r0, #0x18]
    ands r1, r2, #0x40
    beq _2c4
    ands r1, r2, #2
    beq _2d4
    ldr r0, [r0, #0x10]
    cmp r0, #1
    bne _2d4
_2c4:
    ldr r0, [pc, #0x2d4]
    mov r1, #0
    str r1, [r0, #0x20]
    str r1, [r0, #0x24]
_2d4:
    ldr r0, [pc, #0x2c4]
    ldr r1, [r0, #0x18]
    ands r1, r1, #0x20
    beq _374
    bl WirelessIcon_Create
    bl WH_GetCurrentAid
    bl NetPacket_FindSlot
    ldr r2, [pc, #0x2a8]
    mov r1, r0
    ldr r0, [r2]
    mov r0, r0, lsl #0x10
    mov r0, r0, lsr #0x10
    bl Player_Create
    mov r5, #0
    ldr r4, [pc, #0x28c]
_310:
    bl WH_GetCurrentAid
    cmp r5, r0
    beq _33c
    mov r0, r5
    bl NetPacket_FindSlot
    add r1, r4, r5, lsl #2
    ldr r2, [r1, #4]
    mov r1, r0
    mov r0, r2, lsl #0x10
    mov r0, r0, lsr #0x10
    bl Player_Create
_33c:
    add r0, r5, #1
    mov r0, r0, lsl #0x10
    mov r5, r0, lsr #0x10
    cmp r5, #2
    blo _310
    ldr r0, [pc, #0x29c]
    ldr r1, [pc, #0x2a0]
    ldr r2, [r0]
    mov r3, #1
    bic r2, r2, #0x200
    strb r3, [r1, #0x9e]
    str r2, [r0]
    bl FixVsHud_Create
    b _3f8
_374:
    ldr r0, [r0]
    mov r1, #0
    mov r0, r0, lsl #0x10
    mov r0, r0, lsr #0x10
    bl Player_Create
    ldr r0, [pc, #0x264]
    ldr r2, [pc, #0x20c]
    ldr r3, [r0]
    ldr r1, [r2, #0x10]
    orr r3, r3, #0x200
    str r3, [r0]
    cmp r1, #2
    bne _3f8
    ldr r0, [pc, #0x250]
    ldrb r1, [r2, #0x1c]
    ldrb r3, [r0, #0x46]
    cmp r3, r1
    bne _3f8
    ldrb r3, [r0, #0x47]
    ldrb r1, [r2, #0x1d]
    cmp r3, r1
    bne _3f8
    ldr r1, [r0, #0x4c]
    cmp r1, #0
    beq _3f8
    ldr r0, [r0, #0x48]
    ldr r1, [r2, #0x18]
    mov r0, r0, lsl #0x10
    orr r3, r1, #0x80
    mov r0, r0, lsr #0x10
    mov r1, #1
    str r3, [r2, #0x18]
    bl Player_Create
_3f8:
    bl ScreenShake_Create
    bl ObjRect_CheckInit
    bl RingManager_Create
    bl Hud_Create
    ldr r0, [pc, #0x190]
    ldrb r1, [r0, #0x1c]
    cmp r1, #9
    bhs _424
    ldrb r0, [r0, #0x1d]
    cmp r0, #2
    blo _430
_424:
    bl Hud_GetFlags
    orr r0, r0, #4
    bl Hud_SetFlags
_430:
    ldr r0, [pc, #0x168]
    ldr r0, [r0, #0x10]
    sub r0, r0, #1
    cmp r0, #1
    bhi _450
    bl Hud_GetFlags
    orr r0, r0, #0x10
    bl Hud_SetFlags
_450:
    bl StageZoneScript_Create
    ldr r0, [pc, #0x144]
    ldrb r0, [r0, #0x1d]
    cmp r0, #2
    movne r0, #0
    moveq r0, #2
    bl BossCamera_Create
    ldr r1, [pc, #0x190]
    ldr r0, [pc, #0x190]
    str r1, [r0]
    bl ObjCollisionMap_CommitFrame
    bl ObjCollisionMap_CommitFrame
    ldr r0, [pc, #0x118]
    ldr r1, [r0, #0x18]
    bic r1, r1, #1
    str r1, [r0, #0x18]
    bl Stage_InitBlend
    ldr r1, [pc, #0x170]
    ldr r0, [pc, #0x100]
    ldr r2, [r1]
    ldrb r3, [r0, #0x1c]
    orr r2, r2, #1
    str r2, [r1]
    cmp r3, #7
    mov r1, #0
    blo _4cc
    cmp r3, #9
    bhs _4cc
    ldrb r0, [r0, #0x1d]
    cmp r0, #2
    movlo r1, #1
_4cc:
    cmp r1, #0
    beq _4f4
    mov r0, #0x43
    mov r1, #0x100
    bl BrightnessFade_Create
    ldr r0, [pc, #0xb8]
    ldr r1, [r0, #0x18]
    orr r1, r1, #1
    str r1, [r0, #0x18]
    b _55c
_4f4:
    ldr r0, [pc, #0xa4]
    ldr r1, [r0, #0x18]
    ands r0, r1, #8
    bne _514
    ands r0, r1, #0x40
    bne _514
    bl StageTitleCard_Create
    b _55c
_514:
    cmp r3, #9
    bhs _52c
    ldr r0, [pc, #0x7c]
    ldrb r1, [r0, #0x1d]
    cmp r1, #2
    blo _550
_52c:
    bl NaviMsg_Create
    mov r0, #1
    bl NaviMsg_SetVisible
    bl NaviB_Create
    ldr r0, [pc, #0x5c]
    ldr r1, [r0, #0x18]
    orr r1, r1, #1
    str r1, [r0, #0x18]
    b _55c
_550:
    ldr r1, [r0, #0x18]
    orr r1, r1, #1
    str r1, [r0, #0x18]
_55c:
    ldr r2, [pc, #0x3c]
    ldr r0, [r2, #0x18]
    ands r1, r0, #0x40
    addeq sp, sp, #0xc
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bxeq lr
    bic r3, r0, #0x40
    mov r0, #0x43
    mov r1, #0x100
    str r3, [r2, #0x18]
    bl BrightnessFade_Create
    add sp, sp, #0xc
    ldmia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bx lr
    dcd StageNetEvent_Main
    dcd Stage_Main
    dcd Stage_Destructor
    dcd gGameState
    dcd data_02074960
    dcd data_02074962
    dcd data_02074964
    dcd data_020748bc
    dcd data_020748be
    dcd data_020748c0
    dcd data_020748c2
    dcd data_020748c4
    dcd data_020748c6
    dcd data_020748c8
    dcd data_020748ca
    dcd data_020748cc
    dcd data_022c4efc
    dcd 0xfffff9e9
    dcd 0xfffff133
    dcd 0x00000ec7
    dcd data_02074a04
    dcd 0x00000fff
    dcd data_022c4ee8
    dcd data_022c4ef8
    dcd gObjManagerFlags
    dcd data_0207cf78
    dcd gCamera
    dcd data_022c4590
    dcd data_ov000_0236a448
    dcd data_022c4ef4
    dcd gBossStatus
}
