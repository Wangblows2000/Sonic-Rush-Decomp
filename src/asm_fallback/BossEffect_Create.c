// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void MIi_CpuClear16(void);
extern u32 gZoneActArchive;
extern u32 data_ov000_02367b9c;
extern u32 data_ov000_02367c40;
extern u32 data_ov000_02367c48;
extern u32 data_ov000_02367c50;
extern u32 data_ov000_02367c54;
extern void OS_GetArenaLo(void);
extern void NNS_FndGetArchiveFileByName(void);
extern void NNS_FndUnmountArchive(void);
extern void NNS_FndMountArchive(void);
extern void strcat(void);
extern void strcpy(void);
extern void Model_SetAnimation(void);
extern void Task_Create(void);
extern void Asset3D_CreateSetupTask(void);
extern void StageObj_LoadModel(void);
extern void StageObj_Main(void);
extern void BossEffect_Destructor(void);
extern void BossEffect_State_Main(void);

asm void BossEffect_Create(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x8c
    mov r10, r0
    mov r9, r1
    mov r5, #4
    mov r4, r2
    str r3, [sp, #8]
    str r5, [sp]
    mov r5, #0x2b4
    ldr r0, [pc, #0x1fc]
    ldr r1, [pc, #0x1fc]
    mov r2, #0
    mov r3, #0xa00
    str r5, [sp, #4]
    bl Task_Create
    mov r5, r0
    mov r0, #0
    bl OS_GetArenaLo
    cmp r5, r0
    addeq sp, sp, #0x8c
    moveq r0, #0
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxeq lr
    ldr r8, [r5, #0x10]
    mov r0, #0
    strh r0, [sp, #0xc]
    ldrh r0, [sp, #0xc]
    mov r1, r8
    mov r2, #0x2b4
    bl MIi_CpuClear16
    str r4, [r8, #0x2a0]
    ldrh r2, [r8, #0x5c]
    add r0, sp, #0xe
    mov r1, r10
    orr r2, r2, #0x30
    strh r2, [r8, #0x5c]
    mov r2, #2
    str r2, [r8, #8]
    bl strcpy
    ldr r1, [pc, #0x18c]
    add r0, sp, #0xe
    bl strcat
    mov r3, #0
    ldr r1, [pc, #0x180]
    str r3, [sp]
    ldr r2, [r1]
    mov r0, r8
    str r2, [sp, #4]
    add r1, r8, #0x134
    add r2, sp, #0xe
    bl StageObj_LoadModel
    ldr r5, [pc, #0x164]
    add r6, sp, #0xe
    ldrb r4, [r5]
    ldrb r3, [r5, #1]
    ldr r2, [pc, #0x150]
    ldr r1, [pc, #0x154]
    strb r4, [r6]
    strb r3, [r6, #1]
    ldrb r4, [r5, #2]
    ldrb r3, [r5, #3]
    add r0, sp, #0x30
    strb r4, [r6, #2]
    strb r3, [r6, #3]
    ldrb r3, [r5, #4]
    strb r3, [r6, #4]
    ldr r2, [r2]
    bl NNS_FndMountArchive
    add r0, sp, #0x12
    mov r1, r10
    bl strcpy
    ldr r1, [pc, #0x11c]
    add r0, sp, #0x12
    bl strcat
    mov r0, r6
    bl NNS_FndGetArchiveFileByName
    str r0, [r8, #0x2a8]
    ldr r0, [r8, #0x2a0]
    ands r0, r0, #1
    bne _148
    ldr r0, [r8, #0x2a8]
    bl Asset3D_CreateSetupTask
_148:
    mov r7, #0
    add r5, sp, #0x12
    add r11, sp, #0xe
    mov r4, r7
    mov r6, #1
_15c:
    mov r0, r6, lsl r7
    ands r0, r0, r9
    beq _1bc
    mov r0, r5
    mov r1, r10
    bl strcpy
    ldr r0, [pc, #0xc8]
    ldr r1, [r0, r7, lsl #2]
    mov r0, r5
    bl strcat
    cmp r7, #2
    beq _1b0
    mov r0, r11
    bl NNS_FndGetArchiveFileByName
    str r4, [sp]
    mov r2, r0
    add r0, r8, #0x134
    mov r1, r7
    mov r3, r4
    bl Model_SetAnimation
    b _1bc
_1b0:
    mov r0, r11
    bl NNS_FndGetArchiveFileByName
    str r0, [r8, #0x2ac]
_1bc:
    add r7, r7, #1
    cmp r7, #5
    blo _15c
    add r0, sp, #0x30
    bl NNS_FndUnmountArchive
    ldr r0, [sp, #8]
    ldr r2, [sp, #0xb0]
    str r0, [r8, #0x10]
    ldr r1, [sp, #0xb4]
    str r2, [r8, #0x14]
    str r1, [r8, #0x18]
    ldr r0, [pc, #0x58]
    ldr r1, [pc, #0x58]
    str r0, [r8, #0x14c]
    str r0, [r8, #0x150]
    str r0, [r8, #0x154]
    ldr r0, [r8, #0x58]
    orr r0, r0, #0x100
    str r0, [r8, #0x58]
    ldr r2, [r8, #0x54]
    mov r0, r8
    orr r2, r2, #2
    str r2, [r8, #0x54]
    str r1, [r8, #0x68]
    add sp, sp, #0x8c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd StageObj_Main
    dcd BossEffect_Destructor
    dcd data_ov000_02367c40
    dcd gZoneActArchive
    dcd data_ov000_02367c48
    dcd data_ov000_02367c50
    dcd data_ov000_02367c54
    dcd data_ov000_02367b9c
    dcd 0x000034cc
    dcd BossEffect_State_Main
}
