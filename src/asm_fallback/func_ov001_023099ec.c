// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_ov001_023170b8;
extern u32 data_ov001_0231f5bc;
extern u32 data_ov001_0231f5c4;
extern void func_0203344c(void);
extern void func_0203363c(void);
extern void func_0203438c(void);
extern void func_02039c18(void);
extern void func_ov001_02314380(void);

asm void func_ov001_023099ec(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x1c
    ldr r0, [pc, #0x204]
    mov r1, #1
    ldr r6, [r0]
    ldr r0, [pc, #0x1fc]
    str r1, [r6, #0xae8]
    ldr r1, [r0]
    mov r0, #0
    ldr r2, [r1, #0x20]
    ldr r1, [r2, #0x18]
    add r1, r2, r1
    ldrh r1, [r1, #0x30]
    bl func_02039c18
    mov r3, #0
    str r3, [sp]
    str r3, [sp, #4]
    str r0, [sp, #8]
    ldr r1, [pc, #0x1cc]
    str r3, [sp, #0xc]
    ldr r0, [pc, #0x1c0]
    str r1, [sp, #0x10]
    ldr r1, [r0]
    add r0, r6, #0x780
    ldr r1, [r1, #0x20]
    mov r2, #1
    bl func_0203363c
    mov r1, #0
    mov r2, r1
    add r0, r6, #0x780
    bl func_0203344c
    ldr r0, [r6, #0x7ac]
    mov r1, #1
    orr r0, r0, #0x10
    str r0, [r6, #0x7ac]
    add r0, r6, #0x700
    strh r1, [r0, #0xc6]
    ldr r1, [pc, #0x17c]
    mov r0, #0
    ldr r1, [r1]
    ldr r2, [r1, #0x24]
    ldr r1, [r2, #0x18]
    add r1, r2, r1
    ldrh r1, [r1, #6]
    bl func_02039c18
    mov r2, #0
    str r2, [sp]
    str r2, [sp, #4]
    str r0, [sp, #8]
    str r2, [sp, #0xc]
    ldr r1, [pc, #0x14c]
    ldr r0, [pc, #0x14c]
    str r1, [sp, #0x10]
    add r0, r6, r0
    ldr r1, [pc, #0x138]
    mov r3, r2
    ldr r1, [r1]
    ldr r1, [r1, #0x24]
    bl func_0203363c
    ldr r0, [pc, #0x12c]
    mov r1, #0
    add r0, r6, r0
    mov r2, r1
    bl func_0203344c
    mov r1, #1
    add r0, r6, #0x800
    strh r1, [r0, #0xe]
    mov r0, #0x1000
    str r0, [r6, #0xbc0]
    mov r10, #0
    add r9, r6, #0x810
    mov r8, r6
    mov r11, r10
    mov r5, r10
    str r10, [sp, #0x14]
    mov r4, #5
_130:
    ldr r0, [pc, #0xe0]
    mov r1, r10, lsl #1
    ldr r0, [r0]
    ldr r2, [r0, #0x1c]
    ldr r0, [pc, #0xdc]
    ldrh r7, [r0, r1]
    ldr r1, [r2, #0x18]
    mov r0, r11
    add r2, r2, r1
    mov r1, #0x14
    mla r1, r7, r1, r2
    ldrh r1, [r1, #0x1c]
    bl func_02039c18
    str r5, [sp]
    str r5, [sp, #4]
    str r0, [sp, #8]
    ldr r0, [pc, #0xa4]
    str r5, [sp, #0xc]
    str r0, [sp, #0x10]
    ldr r1, [pc, #0x94]
    mov r2, r7
    ldr r1, [r1]
    mov r0, r9
    ldr r1, [r1, #0x1c]
    mov r3, r5
    bl func_0203363c
    ldr r1, [sp, #0x14]
    mov r0, r9
    mov r2, r1
    bl func_0203344c
    ldr r1, [r8, #0x83c]
    add r10, r10, #1
    orr r1, r1, #0x10
    add r0, r8, #0x800
    str r1, [r8, #0x83c]
    strh r4, [r0, #0x56]
    add r9, r9, #0x48
    add r8, r8, #0x48
    cmp r10, #0xa
    blt _130
    add r0, r6, #0xa00
    mov r1, #0x20
    strh r1, [r0, #0x54]
    mov r1, #0xb4
    strh r1, [r0, #0x56]
    ldr r1, [r6, #0xac4]
    add r0, r6, #8
    orr r1, r1, #4
    str r1, [r6, #0xac4]
    bl func_0203438c
    add r0, r6, #0x48
    bl func_0203438c
    bl func_ov001_02314380
    str r0, [r6, #0xae4]
    add sp, sp, #0x1c
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_ov001_0231f5bc
    dcd data_ov001_0231f5c4
    dcd 0x05000200
    dcd 0x000007c8
    dcd data_ov001_023170b8
}
