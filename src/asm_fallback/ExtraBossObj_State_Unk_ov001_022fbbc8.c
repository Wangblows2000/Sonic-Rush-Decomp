// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_0206e470;
extern u32 gRandSeed;
extern u32 gGameState;
extern u32 data_ov001_0231d5d8;
extern u32 data_ov001_0231f094;
extern u32 data_ov001_0231f098;
extern void FX_Sqrt(void);
extern void FX_Atan2(void);
extern void NNS_G3dMdlSetMdlEmiAll(void);
extern void NNS_G3dMdlSetMdlAmbAll(void);
extern void NNS_G3dMdlSetMdlDiffAll(void);
extern void NNS_G3dGetMdlSet(void);
extern void Sound_PlaySeqArc(void);
extern void ExtraBossObj_State_Unk_ov001_022fbb5c(void);
extern void ExtraBoss_Unk_ov001_022fc934(void);
extern void ExtraBoss_GetWork(void);
extern void ExtraBoss_GetPlayer(void);

asm void ExtraBossObj_State_Unk_ov001_022fbbc8(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, lr}
    sub sp, sp, #8
    mov r4, r0
    bl ExtraBoss_GetPlayer
    ldr r1, [r4, #0x1d8]
    mov r8, r0
    sub r0, r1, #1
    str r0, [r4, #0x1d8]
    ldr r0, [r4, #0x1d8]
    cmp r0, #0
    ble _4c
    ands r0, r0, #2
    ldrne r0, [r4, #0x14]
    orrne r0, r0, #0x20000
    strne r0, [r4, #0x14]
    ldreq r0, [r4, #0x14]
    biceq r0, r0, #0x20000
    streq r0, [r4, #0x14]
    b _6c
_4c:
    cmp r0, #0
    bne _6c
    ldr r0, [r4, #0x14]
    bic r0, r0, #0x20000
    str r0, [r4, #0x14]
    ldr r0, [r4, #0xb8]
    bic r0, r0, #0x200
    str r0, [r4, #0xb8]
_6c:
    ldr r0, [r4, #0x1cc]
    sub r0, r0, #1
    str r0, [r4, #0x1cc]
    ldr r0, [r4, #0x1d4]
    sub r0, r0, #1
    str r0, [r4, #0x1d4]
    ldr r1, [r4, #0x1d4]
    cmp r1, #0
    bgt _104
    ldr r0, [r4, #0x14]
    orr r0, r0, #8
    str r0, [r4, #0x14]
    ldr r0, [r4, #0x1c8]
    cmp r0, #8
    ldrne r0, [pc, #0x328]
    ldrne r1, [r0]
    ldrneb r0, [r1]
    bicne r0, r0, #2
    strneb r0, [r1]
    bne _d0
    ldr r0, [pc, #0x310]
    ldr r1, [r0]
    ldrb r0, [r1]
    bic r0, r0, #1
    strb r0, [r1]
_d0:
    mov r0, #0
    str r0, [sp]
    ldr r1, [r4, #0x18]
    ldr r2, [r4, #0x1c]
    mov r3, r0
    bl ExtraBoss_Unk_ov001_022fc934
    mov r0, #0
    mov r1, r0
    mov r2, #0xac
    bl Sound_PlaySeqArc
    add sp, sp, #8
    ldmia sp!, {r4, r5, r6, r7, r8, lr}
    bx lr
_104:
    ldr r0, [r4, #0x1cc]
    cmp r0, #0
    bgt _18c
    cmp r1, #0x78
    ble _18c
    ldr r1, [pc, #0x2b8]
    mov r0, #0
    str r1, [r4, #0x74]
    str r0, [r4, #0x54]
    ldr r1, [r4, #0x54]
    mov r0, #0x1e
    str r1, [r4, #0x50]
    str r0, [r4, #0x1cc]
    bl ExtraBoss_GetWork
    ldr r0, [r0, #0x40]
    cmp r0, #4
    bne _180
    ldr r2, [pc, #0x28c]
    ldr r0, [pc, #0x28c]
    ldr r3, [r2]
    ldr r1, [pc, #0x288]
    mla r1, r3, r0, r1
    mov r0, r1, lsr #0x10
    mov r0, r0, lsl #0x10
    mov r0, r0, lsr #0x10
    ands r0, r0, #1
    movne r0, #1
    str r1, [r2]
    moveq r0, #3
    str r0, [r4, #0x1d0]
    b _2f8
_180:
    mov r0, #1
    str r0, [r4, #0x1d0]
    b _2f8
_18c:
    ldr r3, [r4, #0x18]
    ldr r2, [r8, #0x18]
    ldr r1, [r4, #0x1c]
    ldr r0, [r8, #0x1c]
    sub r7, r3, r2
    sub r5, r1, r0
    smull r1, r3, r7, r7
    mov r0, #0x800
    adds r6, r1, r0
    smull r2, r1, r5, r5
    adc r3, r3, #0
    adds r2, r2, r0
    mov r6, r6, lsr #0xc
    adc r0, r1, #0
    mov r1, r2, lsr #0xc
    orr r6, r6, r3, lsl #20
    orr r1, r1, r0, lsl #20
    add r0, r6, r1
    bl FX_Sqrt
    mov r6, r0
    cmp r6, #0x30000
    mov r2, #0
    ble _200
    mov r0, #0x1e00
    rsb r2, r0, #0
    rsb r0, r6, #0x30000
    cmp r0, r2
    movgt r2, r0
    b _218
_200:
    cmp r6, #0x30000
    bge _218
    rsb r0, r6, #0x30000
    mov r2, #0x1e00
    cmp r0, #0x1e00
    movlt r2, r0
_218:
    mov r0, r5
    mov r1, r7
    add r6, r6, r2
    bl FX_Atan2
    ldr r1, [pc, #0x1b8]
    mov r5, r0
    ldr r0, [r1, #0x10]
    mov r1, #1
    cmp r0, #2
    beq _248
    cmp r0, #3
    movne r1, #0
_248:
    cmp r1, #0
    ldreq r0, [pc, #0x194]
    movne r7, #1
    ldreq r7, [r0, #0x14]
    bl ExtraBoss_GetWork
    ldr r3, [pc, #0x188]
    mov r1, #0xa
    ldr r2, [r0, #0x40]
    mla r0, r7, r1, r3
    mov r1, r2, lsl #1
    ldrh r0, [r1, r0]
    ldr r1, [pc, #0x174]
    ldr r7, [pc, #0x174]
    add r0, r5, r0
    mov r0, r0, lsl #0x10
    and r0, r1, r0, lsr #16
    mov r0, r0, asr #4
    mov r0, r0, lsl #1
    add r1, r0, #1
    mov r1, r1, lsl #1
    ldrsh r2, [r7, r1]
    mov r1, #0x800
    ldr r3, [r8, #0x18]
    smull r5, r2, r6, r2
    adds r5, r5, r1
    adc r2, r2, #0
    mov r5, r5, lsr #0xc
    orr r5, r5, r2, lsl #20
    ldr r2, [r4, #0x18]
    add r3, r5, r3
    sub r2, r3, r2
    str r2, [r4, #0x50]
    mov r0, r0, lsl #1
    ldrsh r3, [r7, r0]
    ldr r2, [r8, #0x1c]
    ldr r0, [r4, #0x1c]
    smull r5, r3, r6, r3
    adds r5, r5, r1
    adc r1, r3, #0
    mov r3, r5, lsr #0xc
    orr r3, r3, r1, lsl #20
    add r1, r3, r2
    sub r0, r1, r0
    str r0, [r4, #0x54]
_2f8:
    ldr r1, [r4, #0x1d4]
    cmp r1, #0x78
    addgt sp, sp, #8
    ldmgtia sp!, {r4, r5, r6, r7, r8, lr}
    bxgt lr
    ands r0, r1, #3
    addne sp, sp, #8
    ldmneia sp!, {r4, r5, r6, r7, r8, lr}
    bxne lr
    ands r0, r1, #4
    beq _37c
    ldr r1, [r4, #0x1c8]
    mov r0, #0x18
    mul r2, r1, r0
    ldr r0, [pc, #0xc0]
    ldr r0, [r0, r2]
    bl NNS_G3dGetMdlSet
    ldrh r2, [r0, #0xe]
    ldr r1, [pc, #0xb4]
    add r2, r0, r2
    ldr r2, [r2, #0xc]
    add r4, r0, r2
    mov r0, r4
    bl NNS_G3dMdlSetMdlDiffAll
    mov r0, r4
    mov r1, #0
    bl NNS_G3dMdlSetMdlAmbAll
    mov r0, r4
    mov r1, #0
    bl NNS_G3dMdlSetMdlEmiAll
    add sp, sp, #8
    ldmia sp!, {r4, r5, r6, r7, r8, lr}
    bx lr
_37c:
    ldr r1, [r4, #0x1c8]
    mov r0, #0x18
    mul r2, r1, r0
    ldr r0, [pc, #0x68]
    ldr r0, [r0, r2]
    bl NNS_G3dGetMdlSet
    ldrh r2, [r0, #0xe]
    ldr r1, [pc, #0x60]
    add r2, r0, r2
    ldr r2, [r2, #0xc]
    add r4, r0, r2
    mov r0, r4
    bl NNS_G3dMdlSetMdlDiffAll
    ldr r1, [pc, #0x48]
    mov r0, r4
    bl NNS_G3dMdlSetMdlAmbAll
    ldr r1, [pc, #0x3c]
    mov r0, r4
    bl NNS_G3dMdlSetMdlEmiAll
    add sp, sp, #8
    ldmia sp!, {r4, r5, r6, r7, r8, lr}
    bx lr
    dcd data_ov001_0231f094
    dcd ExtraBossObj_State_Unk_ov001_022fbb5c
    dcd gRandSeed
    dcd 0x00196225
    dcd 0x3c6ef35f
    dcd gGameState
    dcd data_ov001_0231d5d8
    dcd 0x0000ffff
    dcd data_0206e470
    dcd data_ov001_0231f098
    dcd 0x00007fff
    dcd 0x0000085f
}
