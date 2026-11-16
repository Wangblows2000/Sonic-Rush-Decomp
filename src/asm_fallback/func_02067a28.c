// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_0206e470;
extern u32 data_022c4ddc;
extern u32 data_022c4e64;
extern void func_02000e98(void);
extern void func_02000eb0(void);
extern void func_02067500(void);
extern void func_02067c6c(void);

asm void func_02067a28(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x5c
    ldr r5, [pc, #0x224]
    mov r4, #0
    ldrb r5, [r5]
    mov r10, r0
    mov r9, r1
    ldr r8, [sp, #0x80]
    ldr r7, [sp, #0x84]
    str r2, [sp, #0xc]
    str r3, [sp, #0x10]
    str r10, [sp, #0x14]
    str r9, [sp, #0x18]
    strh r4, [sp, #0x24]
    strh r4, [sp, #0x26]
    cmp r5, #0
    mov r6, #0x18
    addeq sp, sp, #0x5c
    moveq r0, r6
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxeq lr
    cmp r5, #0
    bls _224
    mov r0, r9, lsl #8
    mov r11, r10, lsl #8
    str r0, [sp, #0x1c]
    str r4, [sp, #0x20]
_6c:
    ldr r0, [pc, #0x1c4]
    ldr r5, [r0, r4, lsl #2]
    ldr r0, [r5]
    cmp r0, #0
    beq _20c
    ldrh r0, [r5, #0x20]
    cmp r0, #0
    beq _20c
    ldrh r0, [r5, #0x22]
    cmp r0, #0
    beq _20c
    mov r0, r5
    add r1, sp, #0x24
    add r2, sp, #0x26
    ldr r10, [sp, #0x14]
    ldr r9, [sp, #0x18]
    bl func_02067c6c
    ldrh r0, [r5, #0xc]
    ands r0, r0, #1
    beq _174
    ldr r1, [r5]
    ldrb r0, [r1, #1]
    cmp r0, #0
    beq _174
    ldr r2, [r1, #0x14]
    ldr r0, [sp, #0x1c]
    ldr r1, [r1, #0x10]
    sub r2, r0, r2
    sub r0, r11, r1
    mov r0, r0, lsl #4
    str r0, [sp, #0x4c]
    mov r0, r2, lsl #4
    str r0, [sp, #0x50]
    ldr r0, [sp, #0x20]
    str r0, [sp, #0x54]
    ldr r1, [r5]
    add r0, sp, #0x28
    ldrb r1, [r1, #1]
    mov r1, r1, lsl #8
    rsb r1, r1, #0
    mov r2, r1, lsl #0x10
    ldr r1, [pc, #0x124]
    and r1, r1, r2, lsr #16
    mov r1, r1, asr #4
    mov r3, r1, lsl #1
    ldr r1, [pc, #0x118]
    mov r2, r3, lsl #1
    ldrsh r1, [r1, r2]
    add r2, r3, #1
    mov r3, r2, lsl #1
    ldr r2, [pc, #0x104]
    ldrsh r2, [r2, r3]
    bl func_02000e98
    add r0, sp, #0x4c
    add r1, sp, #0x28
    mov r2, r0
    bl func_02000eb0
    ldr r0, [r5]
    ldr r2, [sp, #0x4c]
    ldr r1, [r0, #0x10]
    ldr r0, [r0, #0x14]
    add r1, r1, r2, asr #4
    mov r10, r1, asr #8
    ldr r1, [sp, #0x50]
    add r0, r0, r1, asr #4
    mov r9, r0, asr #8
_174:
    ldrsh r2, [sp, #0x24]
    ldr r1, [r5]
    ldr r0, [r1, #0x10]
    mov r0, r0, asr #8
    sub r3, r0, #0x18
    add r3, r3, r2
    cmp r10, r3
    blt _20c
    ldrh r3, [r5, #0x20]
    add r0, r0, r3, lsl #3
    add r0, r0, #0x18
    add r0, r2, r0
    cmp r10, r0
    bgt _20c
    ldr r1, [r1, #0x14]
    ldrsh r0, [sp, #0x26]
    mov r2, r1, asr #8
    sub r1, r2, #0x18
    add r1, r1, r0
    cmp r9, r1
    blt _20c
    ldrh r1, [r5, #0x22]
    add r1, r2, r1, lsl #3
    add r1, r1, #0x18
    add r0, r0, r1
    cmp r9, r0
    bgt _20c
    ldr r3, [sp, #0x10]
    mov r0, r5
    str r3, [sp]
    str r8, [sp, #4]
    ldr r3, [sp, #0xc]
    mov r1, r10
    mov r2, r9
    str r7, [sp, #8]
    bl func_02067500
    cmp r6, r0
    movgt r6, r0
_20c:
    add r0, r4, #1
    and r4, r0, #0xff
    ldr r0, [pc, #0x18]
    ldrb r0, [r0]
    cmp r4, r0
    blo _6c
_224:
    mov r0, r6
    add sp, sp, #0x5c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_022c4ddc
    dcd data_022c4e64
    dcd 0x0000ffff
    dcd data_0206e470
}
