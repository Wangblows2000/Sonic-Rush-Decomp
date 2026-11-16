// asm: fallback — C attempts did not match (best C draft: /home/claude/w_ov001j/src0/func_ov001_022cb5ac.c)
#include <nitro/types.h>

extern u32 data_02072c70;
extern u32 data_02087d84;
extern u32 data_0208f7dc;
extern u32 data_0208f7e0;
extern u32 data_02250434;
extern u32 data_02250438;
extern void func_020337ec(void);
extern void func_0203383c(void);
extern void func_02035704(void);
extern void func_02035754(void);
extern void func_02036bfc(void);

#pragma thumb on
asm void func_ov001_022cb5ac(void)
{
    push {r4, r5, r6, r7, lr}
    sub sp, #0x24
    str r0, [sp]
    add r5, r1, #0
    sub r5, #0x48
    add r6, r2, #7
    cmp r3, #0
    beq _18
    mov r1, #0
    add r0, sp, #0x20
    strh r1, [r0]
    b _1e
_18:
    ldr r1, [pc, #0xd8]
    add r0, sp, #0x20
    strh r1, [r0]
_1e:
    ldr r1, [pc, #0xd8]
    ldr r4, [r1]
    ldr r0, [pc, #0xd8]
    ldr r7, [r0]
    mov r2, #0
    str r2, [r1]
    str r2, [r0]
    add r0, sp, #0x20
    mov r1, #1
    ldr r3, [pc, #0xcc]
    bl func_02035704
    bl func_02035754
    ldr r0, [pc, #0xbc]
    str r4, [r0]
    ldr r0, [pc, #0xbc]
    str r7, [r0]
    mov r4, #0
    str r4, [sp, #8]
    lsl r7, r4, #0
    ldr r0, [pc, #0xb8]
    ldr r0, [r0, #4]
    str r0, [sp, #0x14]
    lsl r0, r4, #0
    str r0, [sp, #0x1c]
    str r0, [sp, #0x18]
_54:
    ldr r1, [sp]
    ldr r0, [pc, #0xb0]
    ldr r0, [r1, r0]
    add r2, r0, r7
    ldr r1, [pc, #0xac]
    ldr r1, [r2, r1]
    str r1, [sp, #4]
    ldr r1, [pc, #0xac]
    ldr r1, [r1]
    str r1, [sp, #0xc]
    ldr r1, [pc, #0xa8]
    ldr r1, [r1]
    str r1, [sp, #0x10]
    ldr r2, [sp, #0x18]
    ldr r1, [pc, #0x9c]
    str r2, [r1]
    ldr r1, [pc, #0x9c]
    str r2, [r1]
    ldr r1, [pc, #0x9c]
    add r0, r0, r1
    add r0, r0, r4
    mov r1, #0x20
    ldr r3, [sp, #4]
    bl func_020337ec
    bl func_0203383c
    ldr r1, [sp, #0xc]
    ldr r0, [pc, #0x80]
    str r1, [r0]
    ldr r1, [sp, #0x10]
    ldr r0, [pc, #0x80]
    str r1, [r0]
    mov r0, #7
    mvn r0, r0
    cmp r5, r0
    ble _dc
    ldr r0, [pc, #0x7c]
    cmp r5, r0
    bge _dc
    mov r0, #7
    mvn r0, r0
    cmp r6, r0
    ble _dc
    cmp r6, #0xc0
    bge _dc
    mov r0, #1
    ldr r1, [sp, #0x1c]
    bl func_02036bfc
    ldr r2, [sp, #4]
    ldr r1, [sp, #0x14]
    sub r1, r2, r1
    ldr r2, [pc, #0x60]
    ldrh r2, [r2, #0x14]
    add r2, r2, #5
    lsr r1, r2
    lsl r2, r6, #0x18
    lsr r2, r2, #0x18
    strh r2, [r0]
    lsl r2, r5, #0x18
    lsr r2, r2, #0x18
    strh r2, [r0, #2]
    ldr r2, [pc, #0x50]
    and r1, r2
    ldr r2, [pc, #0x50]
    orr r1, r2
    strh r1, [r0, #4]
_dc:
    add r5, #8
    add r4, #0x20
    add r7, r7, #4
    ldr r0, [sp, #8]
    add r0, r0, #1
    str r0, [sp, #8]
    cmp r0, #7
    blt _54
    add sp, #0x24
    pop {r4, r5, r6, r7}
    pop {r3}
    bx r3
    dcd 0x0000318c
    dcd data_02250438
    dcd data_02250434
    dcd 0x0500079e
    dcd data_02072c70
    dcd 0x000003ac
    dcd 0x00001de8
    dcd data_0208f7e0
    dcd data_0208f7dc
    dcd 0x00001d08
    dcd 0x00000100
    dcd data_02087d84
    dcd 0x000003ff
    dcd 0x0000c800
}
#pragma thumb reset
