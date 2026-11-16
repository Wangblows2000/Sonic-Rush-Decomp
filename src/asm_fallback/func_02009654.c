// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 data_0207d3f4;
extern u32 data_0207d3f8;
extern u32 data_0207d3fc;
extern u32 data_0207d400;
extern u32 data_0207d404;
extern u32 data_0207d408;
extern u32 data_0207d40c;
extern u32 data_0207d410;
extern u32 data_0207d414;
extern u32 data_0207d440;
extern u32 data_0207d6c0;
extern u32 data_0207e6c0;
extern u32 data_0207eea8;
extern u32 data_0207ef20;
extern void func_0200901c(void);
extern void func_0200927c(void);
extern void func_0200943c(void);
extern void func_02009484(void);
extern void func_0200984c(void);

asm void func_02009654(void)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    bl func_0200901c
    ldr r3, [pc, #0xcc]
    ldr r0, [pc, #0xcc]
    mov r4, #0
    str r3, [r0]
    mov r1, r3
    mov r0, #0x18
_24:
    add r4, r4, #1
    mla r2, r4, r0, r1
    cmp r4, #0xff
    str r2, [r3], #0x18
    blt _24
    ldr r7, [pc, #0xa8]
    mov r10, #0
    ldr r5, [pc, #0xa4]
    ldr r4, [pc, #0xa4]
    ldr lr, [pc, #0xa4]
    ldr r12, [pc, #0xa4]
    ldr r3, [pc, #0xa4]
    ldr r1, [pc, #0xa4]
    ldr r9, [pc, #0xa4]
    ldr r6, [pc, #0xa4]
    ldr r2, [pc, #0xa4]
    mov r8, #1
    ldr r0, [pc, #0xa0]
    ldr r11, [pc, #0xa0]
    str r9, [r6]
    str r10, [r7, #0x7e8]
    str r10, [r5]
    str r10, [r4]
    str r10, [lr]
    str r10, [r12]
    str r10, [r3]
    str r8, [r2]
    str r10, [r1]
    str r0, [r11]
    bl func_0200984c
    mov r0, r8
    bl func_02009484
    cmp r0, #0
    addeq sp, sp, #4
    ldmeqia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxeq lr
    mov r2, #0x1d
    mov r1, r11
    str r2, [r0, #4]
    ldr r1, [r1]
    str r1, [r0, #8]
    bl func_0200943c
    mov r0, r8
    bl func_0200927c
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
    dcd data_0207d6c0
    dcd data_0207d3f4
    dcd data_0207e6c0
    dcd data_0207d3fc
    dcd data_0207d400
    dcd data_0207d410
    dcd data_0207d408
    dcd data_0207d40c
    dcd data_0207d3f8
    dcd data_0207eea8
    dcd data_0207d404
    dcd data_0207d414
    dcd data_0207d440
    dcd data_0207ef20
}
