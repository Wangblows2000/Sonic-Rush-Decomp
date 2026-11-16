// asm: fallback — C attempts did not match (best C draft kept outside the repo)
#include <nitro/types.h>

extern void OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(void);
extern u32 data_022b89e8;
extern void MB_CommResponseRequest(void);
extern void MB_DisconnectChild(void);

asm void func_0203b8f4(void)
{
    stmdb sp!, {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r1, #1
    mov r4, r0
    bl MB_CommResponseRequest
    cmp r0, #0
    addne sp, sp, #4
    ldmneia sp!, {r4, r5, r6, r7, lr}
    bxne lr
    mov r0, #1
    mvn r0, r0, lsl r4
    mov r5, r0, lsl #0x10
    bl OS_DisableInterrupts
    ldr r3, [pc, #0x5c]
    ldrh r2, [r3, #2]
    ldrh r1, [r3, #4]
    ldrh r7, [r3, #6]
    ldrh r6, [r3, #8]
    ldrh lr, [r3, #0xa]
    and r2, r2, r5, lsr #16
    and r1, r1, r5, lsr #16
    and r7, r7, r5, lsr #16
    and r6, r6, r5, lsr #16
    and lr, lr, r5, lsr #16
    ldrh r12, [r3, #0xc]
    strh r2, [r3, #2]
    strh r1, [r3, #4]
    and r5, r12, r5, lsr #16
    strh r7, [r3, #6]
    strh r6, [r3, #8]
    strh lr, [r3, #0xa]
    strh r5, [r3, #0xc]
    bl OS_RestoreInterrupts
    mov r0, r4
    bl MB_DisconnectChild
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
    dcd data_022b89e8
}
