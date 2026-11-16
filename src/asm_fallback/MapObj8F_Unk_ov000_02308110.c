// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gCamera;
extern void func_ov000_02307e84(void);
extern void MapObj8F_State_Unk_ov000_02307e9c(void);
extern void func_ov000_0233b250(void);
extern void func_ov000_0234237c(void);

asm void MapObj8F_Unk_ov000_02308110(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0xc
    mov r10, r0
    bl func_ov000_02307e84
    mov r1, r0
    mov r0, #1
    bl func_ov000_0234237c
    movs r9, r0
    ldr r1, [r10, #0x1f4]
    addeq sp, sp, #0xc
    orr r0, r1, #0x10
    str r0, [r10, #0x1f4]
    ldreq r0, [r10, #0x54]
    orreq r0, r0, #4
    streq r0, [r10, #0x54]
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxeq lr
    ldr r0, [r10, #0x1e0]
    ldrh r0, [r0, #4]
    ands r0, r0, #2
    ldrne r0, [r10, #0x54]
    addne sp, sp, #0xc
    orrne r0, r0, #4
    strne r0, [r10, #0x54]
    ldmneia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxne lr
    mov r0, #0x10
    str r0, [sp]
    mov r0, #0xa
    ldr r4, [pc, #0x120]
    mov r7, #0
    add r5, r10, #0x224
    mov r11, #1
    str r0, [sp, #4]
    mov r6, #2
_8c:
    mov r0, r6, lsl r7
    ldr r1, [r9, #0x20c]
    ands r0, r1, r0
    beq _178
    ldr r1, [r5]
    mov r0, #0x64
    orr r1, r1, r11, lsl r7
    str r1, [r5]
    ldr r1, [r4, #0xc8]
    mla r8, r7, r0, r4
    ands r0, r1, #1
    beq _120
    ldr r0, [r8]
    orr r0, r0, #0x100000
    str r0, [r8]
    ldr r0, [r8]
    orr r0, r0, #0x200000
    str r0, [r8]
    ldr r0, [r8]
    bic r0, r0, #0x400000
    str r0, [r8]
    ldr r0, [r8]
    bic r0, r0, #0x800000
    str r0, [r8]
    ldr r0, [sp]
    strb r0, [r8, #0x1c]
    ldr r0, [sp, #4]
    strb r0, [r8, #0x1d]
    ldr r0, [r4, #0xc8]
    bic r0, r0, #8
    str r0, [r4, #0xc8]
    orr r0, r0, #4
    str r0, [r4, #0xc8]
    ldr r0, [r8]
    orr r0, r0, #0x200
    str r0, [r8]
    b _158
_120:
    ldr r0, [r8]
    add r1, r10, #0x214
    add r2, r10, #0x218
    orr r3, r0, #0x2000
    mov r0, r7
    str r3, [r8]
    add r1, r1, r7, lsl #3
    add r2, r2, r7, lsl #3
    bl func_ov000_0233b250
    add r1, r10, r7, lsl #3
    ldr r0, [r1, #0x214]
    str r0, [r8, #0x3c]
    ldr r0, [r1, #0x218]
    str r0, [r8, #0x40]
_158:
    ldr r0, [r4, #0x114]
    str r0, [r8, #0x4c]
    ldr r0, [r4, #0x118]
    str r0, [r8, #0x50]
    ldr r0, [r4, #0x11c]
    str r0, [r8, #0x54]
    ldr r0, [r4, #0x120]
    str r0, [r8, #0x58]
_178:
    add r0, r7, #1
    and r7, r0, #0xff
    cmp r7, #2
    blo _8c
    ldr r0, [pc, #0x10]
    str r0, [r10, #0x68]
    add sp, sp, #0xc
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd gCamera
    dcd MapObj8F_State_Unk_ov000_02307e9c
}
