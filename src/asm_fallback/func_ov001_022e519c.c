// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void _ll_mul(void);
extern void _s32_div_f(void);
extern u32 gSystemFlags;
extern u32 data_02087cdc;
extern u32 data_02087ce0;
extern u32 data_02089264;
extern u32 data_ov001_02316318;
extern void func_02000cb4(void);
extern void func_0200225c(void);
extern void func_02003d94(void);

#pragma thumb on
asm void func_ov001_022e519c(void)
{
    push {r4, r5, r6, r7, lr}
    sub sp, #0x64
    add r4, r0, #0
    str r1, [sp, #8]
    add r7, r2, #0
    str r3, [sp, #0xc]
    ldr r0, [sp, #0x78]
    str r0, [sp, #0x78]
    cmp r7, #0
    bge _16
    mov r7, #0
_16:
    cmp r7, #0xc0
    ble _1c
    mov r7, #0xc0
_1c:
    ldr r0, [sp, #0xc]
    cmp r0, #0
    bge _26
    mov r0, #0
    str r0, [sp, #0xc]
_26:
    ldr r0, [sp, #0xc]
    cmp r0, #0xc0
    ble _30
    mov r0, #0xc0
    str r0, [sp, #0xc]
_30:
    ldr r0, [sp, #0x78]
    cmp r0, #0
    bge _3a
    mov r0, #0
    str r0, [sp, #0x78]
_3a:
    ldr r0, [sp, #0x78]
    cmp r0, #0xc0
    ble _44
    mov r0, #0xc0
    str r0, [sp, #0x78]
_44:
    ldr r0, [sp, #0x7c]
    cmp r0, #0
    bge _4e
    mov r0, #0
    str r0, [sp, #0x7c]
_4e:
    ldr r0, [sp, #0x80]
    cmp r0, #0
    bge _58
    mov r0, #0
    str r0, [sp, #0x80]
_58:
    ldr r0, [sp, #0x84]
    cmp r0, #0
    bge _62
    mov r0, #0
    str r0, [sp, #0x84]
_62:
    ldr r0, [sp, #0x88]
    cmp r0, #1
    bge _6c
    mov r0, #1
    str r0, [sp, #0x88]
_6c:
    ldr r0, [sp, #0x8c]
    cmp r0, #1
    bge _76
    mov r0, #1
    str r0, [sp, #0x8c]
_76:
    ldr r0, [sp, #0x90]
    cmp r0, #1
    bge _80
    mov r0, #1
    str r0, [sp, #0x90]
_80:
    ldr r0, [sp, #0x7c]
    lsl r0, r0, #0xc
    ldr r1, [pc, #0x268]
    bl func_0200225c
    add r5, r0, #0
    ldr r0, [sp, #0x80]
    lsl r0, r0, #0xc
    ldr r1, [pc, #0x25c]
    bl func_0200225c
    str r0, [sp, #0x14]
    ldr r0, [sp, #0x84]
    lsl r0, r0, #0xc
    ldr r1, [pc, #0x250]
    bl func_0200225c
    str r0, [sp, #0x18]
    ldr r0, [pc, #0x248]
    ldr r1, [sp, #0x88]
    lsl r1, r1, #0xc
    bl func_0200225c
    str r0, [sp, #0x1c]
    ldr r0, [pc, #0x23c]
    ldr r1, [sp, #0x8c]
    lsl r1, r1, #0xc
    bl func_0200225c
    str r0, [sp, #0x20]
    ldr r0, [pc, #0x230]
    ldr r1, [sp, #0x90]
    lsl r1, r1, #0xc
    bl func_0200225c
    str r0, [sp, #0x24]
    ldr r0, [pc, #0x228]
    ldr r0, [r0, #0x10]
    lsl r1, r0, #2
    ldr r0, [pc, #0x228]
    ldr r0, [r0, r1]
    str r0, [sp, #0x10]
    cmp r0, #0
    bne _102
    add r0, sp, #0x50
    bl func_02000cb4
    add r0, r4, #0
    mov r1, #8
    bl _s32_div_f
    add r1, #8
    str r1, [sp]
    ldr r0, [sp, #8]
    str r0, [sp, #4]
    ldr r0, [pc, #0x20c]
    add r1, sp, #0x50
    mov r2, #0
    add r3, r2, #0
    bl func_02003d94
    add sp, #0x64
    pop {r4, r5, r6, r7}
    pop {r3}
    bx r3
_102:
    mov r0, #7
    add r1, r4, #0
    bic r1, r0
    sub r0, r4, r1
    add r0, #8
    lsl r6, r0, #8
    mov r1, #0
    cmp r7, #0
    ble _11e
    ldr r0, [sp, #0x10]
_116:
    stmia r0!, {r6}
    add r1, r1, #1
    cmp r1, r7
    blt _116
_11e:
    ldr r0, [sp, #0x94]
    lsl r4, r0, #0xc
    ldr r0, [sp, #0xc]
    cmp r7, r0
    bge _1e4
    sub r0, r0, r7
    lsl r0, r0, #0xc
    str r0, [sp, #0x44]
    ldr r0, [sp, #0x14]
    sub r0, r0, r5
    ldr r1, [sp, #0x44]
    bl func_0200225c
    str r0, [sp, #0x28]
    ldr r1, [sp, #0x20]
    ldr r0, [sp, #0x1c]
    sub r0, r1, r0
    ldr r1, [sp, #0x44]
    bl func_0200225c
    str r0, [sp, #0x2c]
    ldr r1, [sp, #0x1c]
    str r1, [sp, #0x30]
    ldr r0, [sp, #8]
    mul r1, r0
    str r1, [sp, #0x30]
    ldr r0, [sp, #0xc]
    cmp r7, r0
    bge _1e4
    lsl r1, r7, #2
    ldr r0, [sp, #0x10]
    add r0, r0, r1
    str r0, [sp, #0x3c]
    ldr r1, [sp, #8]
    str r1, [sp, #0x40]
    ldr r0, [sp, #0x2c]
    mul r1, r0
    str r1, [sp, #0x40]
_16a:
    ldr r0, [sp, #0x30]
    add r0, r4, r0
    asr r0, r0, #0xc
    ldr r1, [pc, #0x18c]
    bl _s32_div_f
    cmp r1, #0x80
    blt _17e
    mov r0, #0xff
    sub r1, r0, r1
_17e:
    cmp r1, #0x40
    bge _18a
    ldr r0, [pc, #0x180]
    ldrb r0, [r0, r1]
    lsl r1, r0, #0xc
    b _198
_18a:
    mov r0, #0x7f
    sub r1, r0, r1
    ldr r0, [pc, #0x174]
    ldrb r1, [r0, r1]
    mov r0, #0xfe
    sub r0, r0, r1
    lsl r1, r0, #0xc
_198:
    ldr r0, [pc, #0x16c]
    sub r0, r1, r0
    asr r1, r0, #0x1f
    asr r3, r5, #0x1f
    add r2, r5, #0
    bl _ll_mul
    ldr r2, [pc, #0x164]
    add r0, r0, r2
    mov r2, #0
    adc r1, r2
    lsl r1, r1, #0x14
    lsr r0, r0, #0xc
    orr r0, r1
    asr r0, r0, #0xc
    lsl r0, r0, #8
    add r1, r6, r0
    ldr r0, [sp, #0x3c]
    str r1, [r0]
    ldr r0, [sp, #0x28]
    add r5, r5, r0
    ldr r0, [sp, #0x1c]
    add r4, r4, r0
    ldr r1, [sp, #0x30]
    ldr r0, [sp, #0x40]
    add r0, r1, r0
    str r0, [sp, #0x30]
    ldr r1, [sp, #0x1c]
    ldr r0, [sp, #0x2c]
    add r0, r1, r0
    str r0, [sp, #0x1c]
    ldr r0, [sp, #0x3c]
    add r0, r0, #4
    str r0, [sp, #0x3c]
    add r7, r7, #1
    ldr r0, [sp, #0xc]
    cmp r7, r0
    blt _16a
_1e4:
    ldr r1, [sp, #0xc]
    ldr r0, [sp, #0x78]
    cmp r1, r0
    bge _2a8
    lsl r1, r0, #0
    ldr r0, [sp, #0xc]
    sub r0, r1, r0
    lsl r7, r0, #0xc
    ldr r1, [sp, #0x18]
    ldr r0, [sp, #0x14]
    sub r0, r1, r0
    add r1, r7, #0
    bl func_0200225c
    str r0, [sp, #0x48]
    ldr r1, [sp, #0x24]
    ldr r0, [sp, #0x20]
    sub r0, r1, r0
    add r1, r7, #0
    bl func_0200225c
    str r0, [sp, #0x4c]
    ldr r1, [sp, #0x20]
    str r1, [sp, #0x34]
    ldr r0, [sp, #8]
    mul r1, r0
    str r1, [sp, #0x34]
    ldr r1, [sp, #0xc]
    ldr r0, [sp, #0x78]
    cmp r1, r0
    bge _2a8
    lsl r1, r1, #2
    ldr r0, [sp, #0x10]
    add r7, r0, r1
    ldr r1, [sp, #8]
    str r1, [sp, #0x38]
    ldr r0, [sp, #0x4c]
    mul r1, r0
    str r1, [sp, #0x38]
_232:
    ldr r0, [sp, #0x34]
    add r0, r4, r0
    asr r0, r0, #0xc
    ldr r1, [pc, #0xc4]
    bl _s32_div_f
    cmp r1, #0x80
    blt _246
    mov r0, #0xff
    sub r1, r0, r1
_246:
    cmp r1, #0x40
    bge _252
    ldr r0, [pc, #0xb8]
    ldrb r0, [r0, r1]
    lsl r1, r0, #0xc
    b _260
_252:
    mov r0, #0x7f
    sub r1, r0, r1
    ldr r0, [pc, #0xac]
    ldrb r1, [r0, r1]
    mov r0, #0xfe
    sub r0, r0, r1
    lsl r1, r0, #0xc
_260:
    ldr r0, [pc, #0xa4]
    sub r0, r1, r0
    asr r1, r0, #0x1f
    asr r3, r5, #0x1f
    add r2, r5, #0
    bl _ll_mul
    ldr r2, [pc, #0x9c]
    add r0, r0, r2
    mov r2, #0
    adc r1, r2
    lsl r1, r1, #0x14
    lsr r0, r0, #0xc
    orr r0, r1
    asr r0, r0, #0xc
    lsl r0, r0, #8
    add r0, r6, r0
    stmia r7!, {r0}
    ldr r0, [sp, #0x48]
    add r5, r5, r0
    ldr r0, [sp, #0x20]
    add r4, r4, r0
    ldr r1, [sp, #0x34]
    ldr r0, [sp, #0x38]
    add r0, r1, r0
    str r0, [sp, #0x34]
    ldr r1, [sp, #0x20]
    ldr r0, [sp, #0x4c]
    add r0, r1, r0
    str r0, [sp, #0x20]
    ldr r0, [sp, #0xc]
    add r1, r0, #1
    str r1, [sp, #0xc]
    ldr r0, [sp, #0x78]
    cmp r1, r0
    blt _232
_2a8:
    ldr r0, [sp, #0x78]
    cmp r0, #0xc0
    bge _2c0
    lsl r1, r0, #2
    ldr r0, [sp, #0x10]
    add r1, r0, r1
_2b4:
    stmia r1!, {r6}
    ldr r0, [sp, #0x78]
    add r0, r0, #1
    str r0, [sp, #0x78]
    cmp r0, #0xc0
    blt _2b4
_2c0:
    add r0, sp, #0x50
    bl func_02000cb4
    ldr r0, [sp, #0x10]
    ldr r0, [r0]
    lsr r0, r0, #8
    str r0, [sp]
    ldr r0, [sp, #8]
    str r0, [sp, #4]
    ldr r0, [pc, #0x28]
    add r1, sp, #0x50
    mov r2, #0
    add r3, r2, #0
    bl func_02003d94
    ldr r1, [pc, #0x30]
    ldr r2, [r1]
    ldr r0, [pc, #0x30]
    orr r2, r0
    str r2, [r1]
    add sp, #0x64
    pop {r4, r5, r6, r7}
    pop {r3}
    bx r3
    dcd 0x00100000
    dcd data_02087cdc
    dcd data_02087ce0
    dcd data_02089264
    dcd 0x00000100
    dcd data_ov001_02316318
    dcd 0x0007f000
    dcd 0x00000800
    dcd gSystemFlags
    dcd 0x00001000
}
#pragma thumb reset
