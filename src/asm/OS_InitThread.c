// asm: C-impossible: linker constants in pool with no relocs (raw dcd words)
#include <nitro/types.h>

extern void OSi_IdleThreadProc(void);
extern u32 data_0207d084;
extern u32 data_0207d088;
extern u32 data_0207d090;
extern u32 data_0207d098;
extern u32 data_0207d0e4;
extern u32 data_0207d178;
extern u32 data_0207d2d4;
extern u32 data_027e0000;
extern void OS_SetSwitchThreadCallback(void);
extern void OS_CreateThread(void);

asm void OS_InitThread(void)
{
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #0xc
    ldr r0, [pc, #0x134]
    ldr r1, [r0]
    cmp r1, #0
    addne sp, sp, #0xc
    ldmneia sp!, {r4, r5, lr}
    bxne lr
    mov r1, #1
    str r1, [r0]
    mov r1, #0
    ldr r4, [pc, #0x114]
    mov r5, r1
_34:
    add r0, r4, r1, lsl #2
    add r1, r1, #1
    str r5, [r0, #0x14]
    cmp r1, #0x10
    blt _34
    ldr r0, [pc, #0xfc]
    ldr r2, [pc, #0xfc]
    mov r12, #0x10
    mov r3, #1
    str r12, [r0, #0x70]
    ldr lr, [pc, #0xf0]
    ldr r1, [pc, #0xf0]
    str r5, [r0, #0x6c]
    str r3, [r0, #0x64]
    str r5, [r0, #0x68]
    str r5, [r0, #0x74]
    str r0, [r4, #0x14]
    str r0, [r4, #0xc]
    str r0, [r4, #8]
    cmp r2, #0
    ldreq r0, [pc, #0xd0]
    str lr, [r1]
    ldrne r1, [pc, #0xcc]
    subeq r4, r0, r2
    ldrne r0, [pc, #0xc8]
    addne r1, r1, #0x3f80
    subne r0, r1, r0
    subne r4, r0, r2
    ldr r1, [pc, #0xb4]
    ldr r2, [pc, #0x9c]
    ldr r0, [pc, #0xb0]
    add r1, r1, #0x3f80
    sub r3, r1, r0
    mov r0, #0
    ldr r1, [pc, #0xa4]
    str r3, [r2, #0x88]
    str r4, [r2, #0x84]
    str r0, [r2, #0x8c]
    str r1, [r3, #-4]
    ldr r3, [r2, #0x84]
    ldr r12, [pc, #0x90]
    ldr r1, [pc, #0x68]
    str r12, [r3]
    mov r12, #0x10
    ldr r3, [pc, #0x84]
    strh r0, [r2, #0x90]
    strh r12, [r1, #2]
    strh r0, [r1]
    strh r0, [r1, #4]
    str r1, [r3]
    bl OS_SetSwitchThreadCallback
    mov r2, #0xc8
    str r2, [sp]
    mov r12, #0x1f
    ldr r0, [pc, #0x60]
    ldr r1, [pc, #0x60]
    ldr r3, [pc, #0x60]
    mov r2, #0
    str r12, [sp, #4]
    bl OS_CreateThread
    ldr r0, [pc, #0x48]
    mov r2, #0x20
    mov r1, #1
    str r2, [r0, #0x70]
    str r1, [r0, #0x64]
    add sp, sp, #0xc
    ldmia sp!, {r4, r5, lr}
    bx lr
    dcd data_0207d088
    dcd data_0207d090
    dcd data_0207d178
    dcd 0x00000000
    dcd data_0207d098
    dcd data_0207d084
    dcd 0x027e2060
    dcd data_027e0000
    dcd 0x00000800
    dcd 0xfddb597d
    dcd 0x7bf9dd5b
    dcd 0x027fffa0
    dcd data_0207d0e4
    dcd OSi_IdleThreadProc
    dcd data_0207d2d4
}
