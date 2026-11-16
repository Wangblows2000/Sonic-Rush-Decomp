// asm: hand-written runtime library routine (split at mid-function entry points, see w_asm/split_plan.txt)
#include <nitro/types.h>

extern void _ll_sdiv_e10(void);
extern void _ll_sdiv_e74(void);
extern void _u32_div_f_e8(void);
void _ull_mod(void);
void _ull_mod_e8(void);

asm void _ull_mod(void)
{
    stmdb sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, #1
}

asm void _ull_mod_e8(void)
{
    orrs r5, r3, r2
    bne _10
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
_10:
    orrs r5, r1, r3
    bne _ll_sdiv_e74
    mov r1, r2
    bl _u32_div_f_e8
    cmp r4, #0
    movne r0, r1
    mov r1, #0
    ldmia sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
    stmdb sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, r1
    orr r4, r4, #1
    b _ll_sdiv_e10
}
