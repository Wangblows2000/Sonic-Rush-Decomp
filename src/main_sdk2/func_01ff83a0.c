#include <nitro/types.h>

void DC_InvalidateAll(void);
void DC_StoreAll(void);
void DC_WaitWriteBufferEmpty(void);
void IC_InvalidateAll(void);
void OS_DisableInterrupts(void);
void OS_RestoreInterrupts(void);
void func_01ff8270(void);

asm void func_01ff83a0(void)
{
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    ldr r0, =0x27ffc2c
    ldr r10, [r0]
    cmp r10, #0x8000
    blo @1
    ldr r1, =0x27ffe00
    mov r0, r10
    mov r2, #0x160
    bl func_01ff8270
@1:
    ldr r5, =0x27ffe20
    ldr r4, =0x27ffe28
    ldr r3, =0x27ffe2c
    ldr r2, =0x27ffe30
    ldr r1, =0x27ffe38
    ldr r9, [r5]
    ldr r0, =0x27ffe3c
    ldr r8, [r4]
    ldr r7, [r3]
    ldr r6, [r2]
    ldr r5, [r1]
    ldr r4, [r0]
    bl OS_DisableInterrupts
    mov r11, r0
    bl DC_StoreAll
    bl DC_InvalidateAll
    mov r0, r11
    bl OS_RestoreInterrupts
    bl IC_InvalidateAll
    bl DC_WaitWriteBufferEmpty
    add r9, r9, r10
    cmp r9, #0x8000
    rsblo r0, r9, #0x8000
    addlo r8, r8, r0
    sublo r7, r7, r0
    movlo r9, #0x8000
    add r6, r6, r10
    mov r0, r9
    mov r1, r8
    mov r2, r7
    bl func_01ff8270
    mov r0, r6
    mov r1, r5
    mov r2, r4
    bl func_01ff8270
    add sp, sp, #4
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
}

