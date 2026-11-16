// asm: fallback — C attempts did not match (best C draft: /home/claude/w_ov000q/de/vol.c)
#include <nitro/types.h>

extern u32 data_ov000_02369ac0;
extern u32 data_ov000_02369b2c;
extern u32 data_ov000_02369b30;
extern u32 data_ov000_0236a444;
extern u32 data_ov000_0236a524;
extern u32 data_ov000_0236ac60;
extern void func_02031bd0(void);
extern void func_02031c1c(void);
extern void func_020533e0(void);
extern void func_020537b4(void);
extern void func_ov000_022dcf20(void);
extern void func_ov000_022de304(void);
extern void func_ov000_022de65c(void);
extern void func_ov000_022de768(void);
extern void func_ov000_022dea0c(void);
extern void func_ov000_022fad3c(void);

asm void func_ov000_022deab4(void)
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
    bl func_02031c1c
    ldr r1, [pc, #0xd0]
    mov r0, r5
    ldr r1, [r1, #0x18]
    mov r2, #0
    bl func_02031bd0
    ldr r0, [pc, #0xc0]
    str r0, [r5, #0x20]
    ldr r0, [r5, #0x20]
    str r0, [r5, #0x1c]
    ldr r0, [r5, #0x1c]
    str r0, [r5, #0x18]
    bl func_ov000_022de65c
    mov r1, #0
    str r1, [sp]
    str r1, [sp, #4]
    str r1, [sp, #8]
    str r1, [sp, #0xc]
    str r1, [sp, #0x10]
    mov r0, #0xce
    mov r2, r1
    mov r3, r1
    bl func_ov000_022fad3c
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
    bl func_ov000_022fad3c
    str r0, [r4, #0x210]
    mov r0, r4
    bl func_ov000_022dcf20
    mov r0, #9
    bl func_020537b4
    mov r0, #0xf
    mov r1, #0xf0
    bl func_020533e0
    ldr r1, [pc, #0x34]
    mov r0, r4
    str r1, [r6, #0x68]
    bl func_ov000_022de304
    add sp, sp, #0x18
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    dcd data_ov000_02369ac0
    dcd data_ov000_02369b2c
    dcd data_ov000_0236ac60
    dcd func_ov000_022de768
    dcd data_ov000_0236a444
    dcd data_ov000_0236a524
    dcd data_ov000_02369b30
    dcd 0x01355b50
    dcd func_ov000_022dea0c
}
