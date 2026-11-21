// asm: hand-written runtime library routine (split at mid-function entry points, see w_asm/split_plan.txt)
#include <nitro/types.h>

extern void _ull_mod_e8(void);
void _ll_udiv(void);

asm void _ll_udiv(void)
{
    stmdb sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, #0
    b _ull_mod_e8
}
