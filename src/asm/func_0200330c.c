// asm: C-impossible: switch range grouping
#include <nitro/types.h>

extern void GX_VRAMCNT_SetLCDC_(void);
extern u32 data_0207cfe8;

asm void func_0200330c(void)
{
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, [pc, #0x8c]
    mvn r12, r0
    ldrh r2, [r1, #0x12]
    ldrh r3, [r1]
    cmp r0, #0x80
    strh r0, [r1, #0x12]
    orr r2, r3, r2
    and r2, r12, r2
    strh r2, [r1]
    bgt _64
    cmp r0, #0x80
    bge _78
    cmp r0, #4
    bgt _84
    cmp r0, #0
    blt _84
    cmp r0, #0
    beq _84
    cmp r0, #4
    ldreq r0, [pc, #0x44]
    moveq r1, #0x84
    streqb r1, [r0]
    b _84
_64:
    cmp r0, #0x180
    bne _84
    ldr r0, [pc, #0x30]
    mov r1, #0x81
    strb r1, [r0]
_78:
    ldr r0, [pc, #0x28]
    mov r1, #0x81
    strb r1, [r0]
_84:
    ldr r0, [pc, #0x10]
    ldrh r0, [r0]
    bl GX_VRAMCNT_SetLCDC_
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
    dcd data_0207cfe8
    dcd 0x04000242
    dcd 0x04000249
    dcd 0x04000248
}
