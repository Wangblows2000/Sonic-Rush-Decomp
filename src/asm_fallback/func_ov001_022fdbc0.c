// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void DC_StoreRange(void);
extern void MIi_CpuClear16(void);
extern void MIi_CpuClear32(void);
extern u32 gMainHeap;
extern u32 data_ov001_02316e94;
extern u32 data_ov001_0231f320;
extern u32 data_ov001_0231f324;
extern void OS_GetArenaLo(void);
extern void AnimatorSprite_ProcessAnimation(void);
extern void AnimatorSprite_Init(void);
extern void Heap_Alloc(void);
extern void ObjVram_Alloc(void);

asm void func_ov001_022fdbc0(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x24
    ldr r0, [pc, #0x268]
    mov r1, #0x7e0
    ldr r0, [r0]
    mov r2, #0x20
    bl Heap_Alloc
    mov r4, r0
    mov r0, #0
    bl OS_GetArenaLo
    cmp r0, r4
    beq _64
    ldr r0, [pc, #0x244]
    mov r1, r4
    str r0, [sp, #0x1c]
    ldr r0, [sp, #0x1c]
    mov r2, #0x7e0
    bl MIi_CpuClear32
    mov r1, #0x7e0
    ldr r2, [pc, #0x22c]
    str r1, [r4]
    mov r0, r4
    str r2, [r4, #0x7dc]
    bl DC_StoreRange
    add r4, r4, #0x20
_64:
    mov r0, #0
    strh r0, [sp, #0x18]
    ldrh r0, [sp, #0x18]
    ldr r3, [pc, #0x20c]
    mov r1, r4
    mov r2, #0x7a0
    str r4, [r3]
    bl MIi_CpuClear16
    ldr r0, [pc, #0x1f8]
    mov r10, #0
    ldr r9, [r0]
    ldr r11, [pc, #0x1f0]
    mov r7, r10
    mov r6, r10
    str r10, [sp, #0x14]
    mov r5, #0x60
    mov r4, #1
_a8:
    ldr r3, [r11]
    ldr r0, [pc, #0x1d8]
    mov r2, r10, lsl #1
    ldr r1, [r3, #0x18]
    ldrh r8, [r0, r2]
    add r1, r3, r1
    mov r0, #0x14
    mla r0, r8, r0, r1
    ldrh r1, [r0, #0x1e]
    mov r0, r7
    bl ObjVram_Alloc
    str r6, [sp]
    str r6, [sp, #4]
    str r0, [sp, #8]
    ldr r0, [pc, #0x1a8]
    str r6, [sp, #0xc]
    str r0, [sp, #0x10]
    ldr r1, [r11]
    mov r2, r8
    mov r0, r9
    mov r3, r5
    bl AnimatorSprite_Init
    ldr r1, [sp, #0x14]
    mov r0, r9
    mov r2, r1
    strh r4, [r9, #0x44]
    bl AnimatorSprite_ProcessAnimation
    add r10, r10, #1
    cmp r10, #0x18
    add r9, r9, #0x48
    blt _a8
    cmp r10, #0x1b
    bge _1b8
    ldr r4, [pc, #0x154]
    ldr r11, [pc, #0x154]
    mov r7, #0
    mov r6, #0x60
    mov r5, #2
_140:
    ldr r2, [r4]
    mov r1, r10, lsl #1
    ldr r0, [r2, #0x18]
    ldrh r8, [r11, r1]
    add r1, r2, r0
    mov r0, #0x14
    mla r0, r8, r0, r1
    ldrh r1, [r0, #0x1e]
    mov r0, r7
    bl ObjVram_Alloc
    str r7, [sp]
    str r7, [sp, #4]
    str r0, [sp, #8]
    ldr r0, [pc, #0x114]
    str r7, [sp, #0xc]
    str r0, [sp, #0x10]
    ldr r1, [r4]
    mov r2, r8
    mov r0, r9
    mov r3, r6
    bl AnimatorSprite_Init
    mov r0, r9
    mov r1, r7
    mov r2, r7
    strh r5, [r9, #0x44]
    bl AnimatorSprite_ProcessAnimation
    add r10, r10, #1
    cmp r10, #0x1b
    add r9, r9, #0x48
    blt _140
_1b8:
    ldr r1, [pc, #0xc4]
    mov r5, #0
    ldr r0, [r1]
    mov r4, #2
    add r0, r0, #0x500
    strh r5, [r0, #0xa4]
    ldr r0, [r1]
    mov r3, #0x5c
    add r0, r0, #0x500
    strh r4, [r0, #0xa6]
    ldr r0, [r1]
    mov r2, #0x12
    add r0, r0, #0x500
    strh r5, [r0, #0xec]
    ldr r0, [r1]
    mov r5, #4
    add r0, r0, #0x500
    strh r4, [r0, #0xee]
    ldr r0, [r1]
    mov r4, #0xaa
    add r0, r0, #0x600
    strh r3, [r0, #0x34]
    ldr r0, [r1]
    mov r3, #0x1c
    add r0, r0, #0x600
    strh r2, [r0, #0x36]
    ldr r0, [r1]
    mov r2, #0xbc
    add r0, r0, #0x700
    strh r5, [r0, #0xc]
    ldr r0, [r1]
    add r0, r0, #0x700
    strh r4, [r0, #0xe]
    ldr r0, [r1]
    add r0, r0, #0x700
    strh r5, [r0, #0x54]
    ldr r0, [r1]
    add r0, r0, #0x700
    strh r4, [r0, #0x56]
    ldr r0, [r1]
    add r0, r0, #0x600
    strh r3, [r0, #0xc4]
    ldr r0, [r1]
    add r0, r0, #0x600
    strh r2, [r0, #0xc6]
    add sp, sp, #0x24
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd gMainHeap
    dcd 0xcccccccc
    dcd 0x02467531
    dcd data_ov001_0231f324
    dcd data_ov001_0231f320
    dcd data_ov001_02316e94
    dcd 0x05000200
}
