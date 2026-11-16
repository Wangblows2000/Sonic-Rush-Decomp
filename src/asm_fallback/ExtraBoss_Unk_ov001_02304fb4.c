// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void MIi_CpuCopy16(void);
extern u32 data_0206e470;
extern u32 gSystemFlags;
extern u32 data_02087cf0;
extern u32 gRandSeed;
extern u32 gPaletteTransferTail;
extern u32 gPaletteTransferHead;
extern u32 data_ov001_02316ee0;
extern u32 data_ov001_02316f08;
extern u32 data_ov001_02316f1c;
extern u32 data_ov001_02316f44;
extern u32 data_ov001_02316f94;
extern u32 data_ov001_0231f54c;
extern u32 data_ov001_0231f550;
extern void PaletteTransfer_QueueLoad(void);
extern void PaletteTransfer_Flush(void);
extern void ExtraBoss_GetWork(void);
extern void ExtraBoss_Unk_ov001_02304948(void);
extern void ExtraBoss_Unk_ov001_0230494c(void);

asm void ExtraBoss_Unk_ov001_02304fb4(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    bl ExtraBoss_GetWork
    ldr r6, [r0, #0x40]
    cmp r6, #4
    bge _80
    ldr r0, [pc, #0x310]
    ldr r7, [pc, #0x310]
    ldr r2, [r0]
    ldr r3, [pc, #0x30c]
    ldr r0, [r2, #0x10]
    ldr r5, [r7]
    mov r1, #0
    ldr r4, [r3]
    add r0, r2, r0
    mov r2, #0x5000000
    str r1, [r7]
    str r1, [r3]
    bl PaletteTransfer_QueueLoad
    bl PaletteTransfer_Flush
    mov r2, r7
    ldr r0, [pc, #0x2e0]
    ldr r1, [pc, #0x2d8]
    ldr r3, [r0]
    str r4, [r1]
    ldr r7, [pc, #0x2d4]
    str r5, [r2]
    str r7, [r3, #0x5a4]
    ldr r0, [r0]
    mov r1, #0x14
    str r1, [r0, #0x5a8]
    b _e4
_80:
    ldr r0, [pc, #0x2a8]
    ldr r7, [pc, #0x2a8]
    ldr r2, [r0, #0x2c]
    ldr r3, [pc, #0x2a4]
    ldr r0, [r2, #0x10]
    ldr r5, [r7]
    mov r1, #0
    ldr r4, [r3]
    add r0, r2, r0
    mov r2, #0x5000000
    str r1, [r7]
    str r1, [r3]
    bl PaletteTransfer_QueueLoad
    bl PaletteTransfer_Flush
    mov r2, r7
    ldr r0, [pc, #0x278]
    ldr r1, [pc, #0x270]
    ldr r3, [r0]
    str r4, [r1]
    ldr r7, [pc, #0x270]
    str r5, [r2]
    str r7, [r3, #0x5a4]
    ldr r0, [r0]
    mov r1, #0x14
    str r1, [r0, #0x5a8]
_e4:
    bl ExtraBoss_GetWork
    ldr r1, [pc, #0x24c]
    ldr r3, [r0, #0x40]
    ldr r2, [pc, #0x250]
    ldr r0, [r1]
    ldr r2, [r2, r3, lsl #2]
    mov r3, #0x60000
    str r2, [r0, #0x588]
    ldr r0, [r1]
    mov r2, #0
    str r3, [r0, #0x58c]
    ldr r0, [r1]
    mov r3, #0x20
    add r0, r0, #0x500
    strh r2, [r0, #0x84]
    ldr r0, [r1]
    ldr r2, [pc, #0x220]
    add r0, r0, #0x500
    strh r3, [r0, #0x86]
    ldr r2, [r2, r6, lsl #2]
    ldr r0, [r1]
    str r2, [r0, #0x590]
    bl ExtraBoss_GetWork
    ldr r0, [r0, #0x40]
    mov r10, #0
    cmp r0, #1
    movgt r0, #1
    movle r0, #0
    cmp r0, #0
    ldrne r0, [pc, #0x1dc]
    ldrne r1, [pc, #0x1ec]
    ldrne r0, [r0]
    ldr r2, [pc, #0x1d0]
    strne r1, [r0, #0x598]
    ldreq r0, [pc, #0x1c8]
    ldreq r1, [pc, #0x1dc]
    ldreq r0, [r0]
    mov r3, #0xff
    streq r1, [r0, #0x598]
    ldr r1, [r2]
    ldr r0, [pc, #0x1cc]
    str r10, [r1, #0xb3c]
    ldr r1, [r2]
    ldr r12, [pc, #0x1c4]
    str r10, [r1, #0xb40]
    ldr r4, [r2]
    ldr r1, [pc, #0x1bc]
    str r10, [r4, #0xb44]
    ldr r4, [r0, r6, lsl #2]
    ldr r0, [r2]
    mov r11, r10
    str r4, [r0, #0xb48]
    ldr r0, [r2]
    str r10, [r0, #0x59c]
    ldr r0, [r2]
    str r10, [r0, #0x5a0]
    ldr r0, [r2]
    add r0, r0, #0xb00
    strh r10, [r0, #0x2c]
    ldr r0, [r2]
    str r10, [r0, #0xb34]
    ldr r0, [r2]
    str r10, [r0, #0xb38]
    ldr r0, [r2]
    str r3, [r0, #0xb30]
    ldr r0, [r2]
    add r8, r0, r1
    ldr r0, [pc, #0x16c]
_1f4:
    ldr r1, [pc, #0x16c]
    ldr r2, [pc, #0x16c]
    ldr r3, [r1]
    ldr r1, [pc, #0x168]
    and r4, r10, #1
    mla r1, r3, r2, r1
    mov r2, r1, lsr #0x10
    mov r3, r2, lsl #0x10
    ldr r2, [pc, #0x158]
    mov r9, r11
    and r2, r2, r3, lsr #16
    add r6, r2, #0x1000
    ldr r2, [pc, #0x13c]
    mov r3, r4, lsl #0x1f
    str r1, [r2]
    mov r7, r3, lsr #0x10
    mov r5, r6, asr #0x1f
_238:
    mov r1, r7, lsl #0x10
    and r1, r0, r1, lsr #16
    mov r1, r1, asr #4
    mov r1, r1, lsl #2
    ldrsh r3, [r12, r1]
    mov r1, #0x800
    add r7, r7, #0x400
    mov r2, r3, asr #0x1f
    add r9, r9, #1
    umull r4, lr, r3, r6
    adds r1, r4, r1
    mla lr, r3, r5, lr
    mla lr, r2, r6, lr
    adc r2, lr, #0
    mov r1, r1, lsr #0xc
    orr r1, r1, r2, lsl #20
    mov r1, r1, asr #9
    strh r1, [r8]
    cmp r9, #0x20
    add r8, r8, #2
    blt _238
    add r10, r10, #1
    cmp r10, #8
    blt _1f4
    ldr r1, [pc, #0x9c]
    ldr r0, [pc, #0xd4]
    ldr r2, [r1]
    ldr r1, [pc, #0xd0]
    add r0, r2, r0
    add r1, r2, r1
    mov r2, #0x180
    bl MIi_CpuCopy16
    ldr r1, [pc, #0x7c]
    ldr r0, [pc, #0xb4]
    ldr r2, [r1]
    ldr r1, [pc, #0xb4]
    add r0, r2, r0
    add r1, r2, r1
    mov r2, #0x180
    bl MIi_CpuCopy16
    ldr r0, [pc, #0x5c]
    ldr r1, [pc, #0x98]
    ldr r4, [r0]
    ldr r0, [pc, #0x94]
    ldr r3, [pc, #0x94]
    ldr r2, [pc, #0x94]
    add r1, r4, r1
    str r2, [r3]
    str r1, [r3, #4]
    add r0, r4, r0
    str r0, [r3, #8]
    mov r0, #2
    str r0, [r3, #0xc]
    mov r1, #0
    ldr r0, [pc, #0x74]
    str r1, [r3, #0x10]
    ldr r1, [r0]
    orr r1, r1, #0x800
    str r1, [r0]
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_ov001_0231f550
    dcd gPaletteTransferHead
    dcd gPaletteTransferTail
    dcd data_ov001_0231f54c
    dcd data_ov001_02316f44
    dcd data_ov001_02316f94
    dcd data_ov001_02316ee0
    dcd data_ov001_02316f1c
    dcd ExtraBoss_Unk_ov001_02304948
    dcd ExtraBoss_Unk_ov001_0230494c
    dcd data_ov001_02316f08
    dcd data_0206e470
    dcd 0x000008ac
    dcd 0x0000ffff
    dcd gRandSeed
    dcd 0x00196225
    dcd 0x3c6ef35f
    dcd 0x00000fff
    dcd 0x000008ec
    dcd 0x000005ac
    dcd 0x0000072c
    dcd data_02087cf0
    dcd 0x04000014
    dcd gSystemFlags
}
