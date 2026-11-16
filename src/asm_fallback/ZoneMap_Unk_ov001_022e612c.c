// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern u32 gTaskCurrent;
extern void SysSound_PlaySE(void);
extern void ZoneMap_Unk_ov001_022e25b0(void);
extern void ZoneMap_Unk_ov001_022e2640(void);
extern void ZoneMap_Unk_ov001_022e5b6c(void);
extern void ZoneMap_Unk_ov001_022e5ba0(void);
extern void ZoneMap_Unk_ov001_022e5f80(void);
extern void ZoneMap_Unk_ov001_022e606c(void);

#pragma thumb on
asm void ZoneMap_Unk_ov001_022e612c(void)
{
    push {lr}
    sub sp, #4
    ldr r0, [pc, #0xbc]
    ldr r0, [r0]
    ldr r2, [r0, #0x10]
    mov r3, #0
    mvn r1, r3
    ldr r0, [r2, #0x14]
    cmp r0, #0xe
    bhi _5a
    add r0, r0, r0
    add r0, pc
    ldrh r0, [r0, #4]
    add r0, pc
    bx r0
    lsl r5, r7, #0
    lsl r7, r3, #0
    lsl r3, r4, #0
    lsl r7, r4, #0
    lsl r5, r7, #0
    lsl r5, r7, #0
    lsl r3, r5, #0
    lsl r5, r7, #0
    lsl r7, r5, #0
    lsl r5, r7, #0
    lsl r5, r7, #0
    lsl r5, r7, #0
    lsl r3, r6, #0
    lsl r7, r6, #0
    lsl r3, r7, #0
    add r1, r3, #0
    b _5a
    mov r1, #1
    b _5a
    mov r1, #2
    b _5a
    mov r1, #3
    b _5a
    mov r1, #4
    b _5a
    mov r1, #5
    b _5a
    mov r1, #6
    b _5a
    mov r1, #7
_5a:
    cmp r1, #0
    bge _82
    ldr r0, [r2]
    cmp r0, #0
    beq _6e
    cmp r0, #1
    beq _78
    add sp, #4
    pop {r3}
    bx r3
_6e:
    bl ZoneMap_Unk_ov001_022e606c
    add sp, #4
    pop {r3}
    bx r3
_78:
    bl ZoneMap_Unk_ov001_022e5f80
    add sp, #4
    pop {r3}
    bx r3
_82:
    mov r0, #0
    ldr r3, [r2]
    cmp r3, #0
    beq _90
    cmp r3, #1
    beq _98
    b _9e
_90:
    ldr r0, [r2, #4]
    bl ZoneMap_Unk_ov001_022e2640
    b _9e
_98:
    ldr r0, [r2, #8]
    bl ZoneMap_Unk_ov001_022e25b0
_9e:
    cmp r0, #0
    beq _b2
    bl ZoneMap_Unk_ov001_022e5b6c
    mov r0, #0xd
    bl SysSound_PlaySE
    add sp, #4
    pop {r3}
    bx r3
_b2:
    bl ZoneMap_Unk_ov001_022e5ba0
    mov r0, #0xb
    bl SysSound_PlaySE
    add sp, #4
    pop {r3}
    bx r3
    mov r8, r8
    dcd gTaskCurrent
}
#pragma thumb reset
