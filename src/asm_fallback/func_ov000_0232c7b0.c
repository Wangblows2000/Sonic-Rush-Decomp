// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void MIi_CpuClear16(void);
extern u32 gRandSeed;
extern u32 gActComArchive;
extern u32 data_ov000_02368040;
extern u32 data_ov000_0236a98c;
extern u32 data_ov000_0236aa18;
extern void func_02006abc(void);
extern void AnimatorSprite_Init(void);
extern void Task_Create(void);
extern void SpritePair_SetOAMOrder(void);
extern void StageObj_Load2dBAC(void);
extern void StageObj_Main(void);
extern void ObjDraw_AllocSpritePalette(void);
extern void func_ov000_0232c4cc(void);
extern void func_ov000_0232c658(void);
extern void func_ov000_0232c794(void);

asm void func_ov000_0232c7b0(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x24
    cmp r0, #2
    addgt sp, sp, #0x24
    mov r2, #0xc
    ldmgtia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxgt lr
    ldr r1, [pc, #0x3e0]
    mov r0, r2, asr r0
    ldr r1, [r1]
    mov r0, r0, lsl #0x10
    cmp r1, #0
    mov r3, r0, lsr #0x10
    bne _264
    mov r0, #4
    str r0, [sp]
    ldr r4, [pc, #0x3c0]
    ldr r0, [pc, #0x3c0]
    ldr r1, [pc, #0x3c0]
    mov r2, #0
    mov r3, #0xa00
    str r4, [sp, #4]
    bl Task_Create
    mov r4, r0
    mov r0, #0
    bl func_02006abc
    cmp r4, r0
    addeq sp, sp, #0x24
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxeq lr
    ldr r0, [pc, #0x384]
    mov r1, #0
    str r4, [r0]
    ldr r5, [r4, #0x10]
    ldr r2, [pc, #0x378]
    strh r1, [sp, #0x20]
    ldrh r0, [sp, #0x20]
    mov r1, r5
    bl MIi_CpuClear16
    ldr r0, [pc, #0x370]
    ldr r1, [pc, #0x370]
    ldr r4, [r0]
    ldr r3, [pc, #0x36c]
    mov r0, r5
    mov r2, #6
    str r4, [sp]
    bl StageObj_Load2dBAC
    mov r4, #0
    mov r8, r4
    mov r6, r4
    mov r0, #5
    str r0, [sp, #0x14]
    mov r0, #2
    str r0, [sp, #0x18]
    mov r7, #1
    mov r0, #9
    str r0, [sp, #0x1c]
_e4:
    str r8, [sp]
    str r8, [sp, #4]
    ldr r0, [r5, #0xc8]
    mov r1, #0x48
    mul r9, r4, r1
    add r0, r0, r4, lsl #6
    str r0, [sp, #8]
    add r1, r5, #0x134
    ldr r0, [pc, #0x314]
    str r8, [sp, #0xc]
    str r0, [sp, #0x10]
    ldr r0, [r5, #0x8c]
    add r10, r1, r9
    ldr r1, [r0]
    mov r0, r10
    mov r2, r4
    mov r3, r8
    bl AnimatorSprite_Init
    str r7, [sp]
    str r6, [sp, #4]
    ldr r1, [r5, #0x110]
    add r0, r5, #0x2e4
    add r1, r1, r4, lsl #6
    str r1, [sp, #8]
    add r11, r0, r9
    ldr r1, [pc, #0x2d4]
    str r6, [sp, #0xc]
    str r1, [sp, #0x10]
    ldr r1, [r5, #0x8c]
    mov r0, r11
    ldr r1, [r1]
    mov r2, r4
    mov r3, r6
    bl AnimatorSprite_Init
    cmp r4, #5
    beq _1a4
    add r0, r5, r9
    add r1, r0, #0x100
    strh r7, [r1, #0x78]
    add r1, r0, #0x300
    strh r7, [r1, #0x28]
    ldr r1, [r0, #0x160]
    orr r1, r1, #0x10
    str r1, [r0, #0x160]
    ldr r1, [r0, #0x310]
    orr r1, r1, #0x10
    str r1, [r0, #0x310]
    b _1e8
_1a4:
    ldr r0, [r5, #0x8c]
    ldr r1, [sp, #0x14]
    ldr r0, [r0]
    ldr r2, [sp, #0x18]
    add r9, r5, r9
    bl ObjDraw_AllocSpritePalette
    add r1, r9, #0x100
    strh r0, [r1, #0x78]
    ldrh r1, [r1, #0x78]
    add r0, r9, #0x300
    strh r1, [r0, #0x28]
    ldr r0, [r9, #0x160]
    orr r0, r0, #0x10
    str r0, [r9, #0x160]
    ldr r0, [r9, #0x310]
    orr r0, r0, #0x10
    str r0, [r9, #0x310]
_1e8:
    ldr r2, [sp, #0x1c]
    mov r0, r10
    mov r1, r11
    bl SpritePair_SetOAMOrder
    add r0, r4, #1
    mov r0, r0, lsl #0x10
    mov r4, r0, lsr #0x10
    cmp r4, #6
    blo _e4
    ldr r1, [pc, #0x214]
    ldr r0, [pc, #0x214]
    str r1, [r5, #0x70]
    ldrh r1, [r5, #0x5c]
    add sp, sp, #0x24
    orr r1, r1, #0x80
    strh r1, [r5, #0x5c]
    ldr r1, [r5, #0x54]
    orr r1, r1, #0x10
    str r1, [r5, #0x54]
    ldr r1, [r5, #0x58]
    orr r1, r1, #0x2100
    str r1, [r5, #0x58]
    ldr r1, [r5, #0x54]
    orr r1, r1, #2
    str r1, [r5, #0x54]
    ldrh r1, [r5, #0x5c]
    orr r1, r1, #4
    strh r1, [r5, #0x5c]
    str r0, [r5, #0x68]
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
_264:
    mov r2, #0
    mov r4, r2
    ldr r1, [r1, #0x10]
    ldr r11, [pc, #0x1b8]
    mov r0, #0xc000
    ldr r8, [pc, #0x1b4]
    ldr r9, [pc, #0x1b4]
    ldr r5, [pc, #0x1b4]
    ldr r6, [pc, #0x1b4]
    b _3f0
_28c:
    cmp r2, #0x20
    bhs _2bc
_294:
    add r7, r1, r2, lsl #1
    add r7, r7, #0x600
    ldrsh r7, [r7, #0x14]
    cmp r7, #0
    beq _2bc
    add r2, r2, #1
    mov r2, r2, lsl #0x10
    mov r2, r2, lsr #0x10
    cmp r2, #0x20
    blo _294
_2bc:
    cmp r2, #0x20
    addeq sp, sp, #0x24
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxeq lr
    ldr r12, [r11]
    add r7, r1, r2, lsl #2
    mla r10, r12, r8, r9
    mov r12, r10, lsr #0x10
    mov r12, r12, lsl #0x10
    mov r12, r12, lsr #0x10
    and r12, r12, #0xff
    str r10, [r11]
    mov r12, r12, lsl #8
    add r10, r1, r2, lsl #3
    str r12, [r10, #0x494]
    str r0, [r10, #0x498]
    ldr lr, [r11]
    add r10, r1, r2, lsl #1
    mla r12, lr, r8, r9
    mov lr, r12, lsr #0x10
    str r12, [r11]
    mov r12, lr, lsl #0x10
    and r12, r5, r12, lsr #16
    sub r12, r6, r12
    add r7, r7, #0x500
    strh r12, [r7, #0x94]
    ldr r12, [r11]
    add r10, r10, #0x600
    mla lr, r12, r8, r9
    mov r12, lr, lsr #0x10
    str lr, [r11]
    mov r12, r12, lsl #0x10
    mov r12, r12, lsr #0x10
    and r12, r12, #0xff
    sub r12, r12, #0x780
    strh r12, [r7, #0x96]
    ldr r12, [r11]
    mla r7, r12, r8, r9
    mov r12, r7, lsr #0x10
    mov r12, r12, lsl #0x10
    mov r12, r12, lsr #0x10
    str r7, [r11]
    and r7, r12, #7
    strh r7, [r10, #0x14]
    ldrsh r7, [r10, #0x14]
    cmp r7, #6
    bne _398
    ldr r12, [r11]
    mla r7, r12, r8, r9
    mov r12, r7, lsr #0x10
    mov r12, r12, lsl #0x10
    mov r12, r12, lsr #0x10
    str r7, [r11]
    and r7, r12, #3
    strh r7, [r10, #0x14]
_398:
    add r7, r1, r2, lsl #1
    add r12, r7, #0x600
    ldrsh r7, [r12, #0x14]
    cmp r7, #7
    bne _3d0
    ldr r10, [r11]
    mla r7, r10, r8, r9
    mov r10, r7, lsr #0x10
    mov r10, r10, lsl #0x10
    mov r10, r10, lsr #0x10
    and r10, r10, #3
    str r7, [r11]
    add r7, r10, #2
    strh r7, [r12, #0x14]
_3d0:
    add r7, r1, r2, lsl #1
    add r7, r7, #0x600
    ldrsh r10, [r7, #0x14]
    add r4, r4, #1
    mov r4, r4, lsl #0x10
    add r10, r10, #1
    strh r10, [r7, #0x14]
    mov r4, r4, lsr #0x10
_3f0:
    cmp r4, r3
    blo _28c
    add sp, sp, #0x24
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_ov000_0236a98c
    dcd 0x00000654
    dcd StageObj_Main
    dcd func_ov000_0232c794
    dcd gActComArchive
    dcd data_ov000_02368040
    dcd data_ov000_0236aa18
    dcd 0x05000200
    dcd 0x05000600
    dcd func_ov000_0232c4cc
    dcd func_ov000_0232c658
    dcd gRandSeed
    dcd 0x00196225
    dcd 0x3c6ef35f
    dcd 0x000007ff
    dcd 0x000003ff
}
