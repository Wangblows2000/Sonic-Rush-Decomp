// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void _s32_div_f(void);
extern void _u32_div_f(void);
extern u32 gTaskCurrent;
extern void SysSound_PlaySE(void);
extern void NaviMsg_HidePanel(void);
extern void ZoneMap_GetDirectionAngle(void);
extern void ZoneMap_SetNodeNaviMessage(void);
extern void ZoneMap_SetCharModelRotY(void);
extern void ZoneMap_SetCharModelAnim(void);
extern void ZoneMap_CalcClampedScroll(void);
extern void ZoneMap_SelectNextState(void);
extern void ZoneMap_UpdateView(void);

#pragma thumb on
asm void ZoneMap_State_MoveToNode(void)
{
    push {r4, r5, r6, r7, lr}
    sub sp, #0x34
    ldr r0, [pc, #0x2e0]
    ldr r0, [r0]
    ldr r5, [r0, #0x10]
    ldr r3, [r5, #0x1c]
    add r0, r5, #0
    add r0, #0xa4
    ldr r0, [r0]
    ldr r1, [pc, #0x2d8]
    add r4, r0, r1
    mov r1, #0x64
    add r2, r3, #0
    mul r2, r1
    add r6, r4, r2
    ldr r1, [pc, #0x2d0]
    add r1, r0, r1
    str r1, [sp, #0xc]
    ldr r1, [r5, #0x18]
    ldr r2, [r5, #0x14]
    cmp r2, r1
    blt _30
    mov r4, #1
    b _32
_30:
    mov r4, #0
_32:
    cmp r3, #0xd
    bne _40
    cmp r4, #0
    bne _3e
    mov r4, #1
    b _40
_3e:
    mov r4, #0
_40:
    cmp r4, #0
    bne _64
    ldr r3, [pc, #0x2ac]
    add r0, r0, r3
    lsl r2, r2, #4
    add r4, r0, r2
    lsl r1, r1, #4
    add r0, r0, r1
    str r0, [sp, #0x28]
    str r0, [sp, #8]
    ldr r0, [r5, #0x74]
    str r0, [sp, #0x2c]
    mov r1, #9
    bl _u32_div_f
    add r2, r0, #1
    add r1, r2, #1
    b _88
_64:
    ldr r3, [pc, #0x28c]
    add r0, r0, r3
    lsl r2, r2, #4
    add r2, r0, r2
    str r2, [sp, #8]
    lsl r1, r1, #4
    add r4, r0, r1
    str r4, [sp, #0x28]
    ldr r0, [r5, #0x74]
    str r0, [sp, #0x2c]
    mov r1, #9
    bl _u32_div_f
    ldr r1, [r6]
    add r1, r1, #1
    sub r0, r1, r0
    sub r2, r0, #1
    sub r1, r2, #1
_88:
    cmp r0, #0
    bne _96
    ldr r0, [r4]
    str r0, [sp, #0x10]
    ldr r0, [r4, #4]
    str r0, [sp, #0x14]
    b _b8
_96:
    ldr r3, [r6]
    add r3, r3, #1
    cmp r0, r3
    bne _ac
    ldr r0, [sp, #8]
    ldr r0, [r0]
    str r0, [sp, #0x10]
    ldr r0, [sp, #8]
    ldr r0, [r0, #4]
    str r0, [sp, #0x14]
    b _b8
_ac:
    lsl r0, r0, #3
    add r3, r6, r0
    ldr r0, [r3, #0x1c]
    str r0, [sp, #0x10]
    ldr r0, [r3, #0x20]
    str r0, [sp, #0x14]
_b8:
    cmp r2, #0
    bne _c4
    ldr r7, [r4]
    ldr r0, [r4, #4]
    str r0, [sp, #0x18]
    b _e0
_c4:
    ldr r0, [r6]
    add r0, r0, #1
    cmp r2, r0
    bne _d6
    ldr r0, [sp, #8]
    ldr r7, [r0]
    ldr r0, [r0, #4]
    str r0, [sp, #0x18]
    b _e0
_d6:
    lsl r0, r2, #3
    add r0, r6, r0
    ldr r7, [r0, #0x1c]
    ldr r0, [r0, #0x20]
    str r0, [sp, #0x18]
_e0:
    mov r0, #0
    mvn r0, r0
    cmp r1, r0
    beq _f0
    ldr r2, [r6]
    add r0, r2, #2
    cmp r1, r0
    bne _104
_f0:
    lsl r1, r7, #1
    ldr r0, [sp, #0x10]
    sub r0, r1, r0
    str r0, [sp, #0x1c]
    ldr r0, [sp, #0x18]
    lsl r1, r0, #1
    ldr r0, [sp, #0x14]
    sub r0, r1, r0
    str r0, [sp, #0x20]
    b _132
_104:
    cmp r1, #0
    bne _112
    ldr r0, [r4]
    str r0, [sp, #0x1c]
    ldr r0, [r4, #4]
    str r0, [sp, #0x20]
    b _132
_112:
    add r0, r2, #1
    cmp r1, r0
    bne _126
    ldr r0, [sp, #8]
    ldr r0, [r0]
    str r0, [sp, #0x1c]
    ldr r0, [sp, #8]
    ldr r0, [r0, #4]
    str r0, [sp, #0x20]
    b _132
_126:
    lsl r0, r1, #3
    add r1, r6, r0
    ldr r0, [r1, #0x1c]
    str r0, [sp, #0x1c]
    ldr r0, [r1, #0x20]
    str r0, [sp, #0x20]
_132:
    ldr r0, [sp, #0x2c]
    mov r1, #9
    bl _u32_div_f
    str r1, [sp, #4]
    ldr r0, [sp, #0x10]
    lsl r4, r0, #0xc
    sub r0, r7, r0
    mul r1, r0
    lsl r0, r1, #0xc
    mov r1, #9
    bl _s32_div_f
    add r4, r4, r0
    ldr r0, [sp, #0x14]
    lsl r0, r0, #0xc
    str r0, [sp, #0x24]
    ldr r1, [sp, #0x18]
    ldr r0, [sp, #0x14]
    sub r0, r1, r0
    ldr r1, [sp, #4]
    mul r1, r0
    lsl r0, r1, #0xc
    mov r1, #9
    bl _s32_div_f
    ldr r1, [sp, #0x24]
    add r0, r1, r0
    str r4, [r5, #0x64]
    str r0, [r5, #0x68]
    ldr r0, [sp, #0x10]
    ldr r1, [sp, #0x14]
    add r2, r7, #0
    ldr r3, [sp, #0x18]
    bl ZoneMap_GetDirectionAngle
    add r4, r0, #0
    add r0, r7, #0
    ldr r1, [sp, #0x18]
    ldr r2, [sp, #0x1c]
    ldr r3, [sp, #0x20]
    bl ZoneMap_GetDirectionAngle
    cmp r4, r0
    blo _1bc
    sub r1, r4, r0
    ldr r0, [pc, #0x168]
    add r3, r1, #0
    and r3, r0
    ldr r2, [pc, #0x164]
    cmp r3, r2
    bhs _1aa
    ldr r0, [sp, #4]
    mul r0, r1
    str r0, [sp, #4]
    mov r1, #9
    bl _u32_div_f
    sub r1, r4, r0
    b _1ea
_1aa:
    sub r1, r0, r1
    ldr r0, [sp, #4]
    mul r0, r1
    str r0, [sp, #4]
    mov r1, #9
    bl _u32_div_f
    add r1, r4, r0
    b _1ea
_1bc:
    sub r1, r0, r4
    ldr r0, [pc, #0x138]
    add r3, r1, #0
    and r3, r0
    ldr r2, [pc, #0x134]
    cmp r3, r2
    bhs _1da
    ldr r0, [sp, #4]
    mul r0, r1
    str r0, [sp, #4]
    mov r1, #9
    bl _u32_div_f
    add r1, r4, r0
    b _1ea
_1da:
    sub r1, r0, r1
    ldr r0, [sp, #4]
    mul r0, r1
    str r0, [sp, #4]
    mov r1, #9
    bl _u32_div_f
    sub r1, r4, r0
_1ea:
    ldr r0, [sp, #0xc]
    bl ZoneMap_SetCharModelRotY
    ldr r0, [sp, #0xc]
    mov r1, #1
    bl ZoneMap_SetCharModelAnim
    add r0, r5, #0
    add r0, #0x70
    str r0, [sp]
    add r0, r5, #0
    add r0, #0xa4
    ldr r0, [r0]
    add r0, r0, #4
    ldr r1, [r5, #0x64]
    asr r1, r1, #0xc
    ldr r2, [r5, #0x68]
    asr r2, r2, #0xc
    add r3, r5, #0
    add r3, #0x6c
    bl ZoneMap_CalcClampedScroll
    add r0, r5, #0
    bl ZoneMap_UpdateView
    ldr r0, [r5]
    cmp r0, #0
    bne _236
    ldr r0, [r5, #0x7c]
    mov r1, #0x1e
    bl _u32_div_f
    cmp r1, #0
    bne _248
    mov r0, #0x10
    bl SysSound_PlaySE
    b _248
_236:
    ldr r0, [r5, #0x7c]
    mov r1, #0x14
    bl _u32_div_f
    cmp r1, #0
    bne _248
    mov r0, #0x11
    bl SysSound_PlaySE
_248:
    ldr r0, [r5, #0x74]
    add r0, r0, #1
    str r0, [r5, #0x74]
    ldr r0, [r5, #0x7c]
    add r0, r0, #1
    str r0, [r5, #0x7c]
    ldr r2, [r5, #0x74]
    ldr r0, [r6]
    add r1, r0, #1
    mov r0, #9
    mul r1, r0
    cmp r2, r1
    blo _2e0
    mov r0, #0
    str r0, [r5, #0x74]
    ldr r0, [r5, #0x18]
    str r0, [r5, #0x14]
    mov r0, #0x17
    str r0, [r5, #0x1c]
    mov r0, #0x12
    str r0, [r5, #0x18]
    add r0, r5, #0
    add r0, #0xa4
    ldr r1, [r0]
    ldr r0, [pc, #0x78]
    add r1, r1, r0
    ldr r0, [r5, #0x14]
    lsl r0, r0, #4
    add r4, r1, r0
    ldr r0, [r1, r0]
    lsl r0, r0, #0xc
    str r0, [r5, #0x64]
    ldr r0, [r4, #4]
    lsl r0, r0, #0xc
    str r0, [r5, #0x68]
    add r0, r5, #0
    add r0, #0x70
    str r0, [sp]
    add r0, r5, #0
    add r0, #0xa4
    ldr r0, [r0]
    add r0, r0, #4
    ldr r1, [sp, #0x28]
    ldr r1, [r1]
    ldr r2, [sp, #0x28]
    ldr r2, [r2, #4]
    add r3, r5, #0
    add r3, #0x6c
    bl ZoneMap_CalcClampedScroll
    add r0, r5, #0
    add r0, #0x90
    ldr r0, [r0]
    cmp r0, #0
    beq _2dc
    ldr r0, [r5, #0x20]
    cmp r0, #0
    bne _2dc
    ldr r0, [r4, #0xc]
    cmp r0, #0
    bne _2cc
    add r0, r5, #0
    ldr r1, [r5, #0x14]
    bl ZoneMap_SetNodeNaviMessage
    b _2dc
_2cc:
    bl NaviMsg_HidePanel
    mov r2, #0x20
    add r5, #0xa4
    ldr r1, [r5]
    ldr r0, [pc, #0x28]
    ldr r0, [r1, r0]
    str r2, [r0]
_2dc:
    bl ZoneMap_SelectNextState
_2e0:
    add sp, #0x34
    pop {r4, r5, r6, r7}
    pop {r3}
    bx r3
    dcd gTaskCurrent
    dcd 0x00000784
    dcd 0x00001994
    dcd 0x00000674
    dcd 0x0000ffff
    dcd 0x00007fff
    dcd 0x00001cc4
}
#pragma thumb reset
