// asm: hand-written runtime library routine (split at mid-function entry points, see w_asm/split_plan.txt)
#include <nitro/types.h>

extern void Runtime_Unk_0202e924(void);
extern void Runtime_Unk_0202e950(void);
extern void Runtime_Unk_0202e974(void);
void Runtime_Unk_0202e910(void);
void Runtime_Unk_0202e990(void);

asm void Runtime_Unk_0202e910(void)
{
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs Runtime_Unk_0202e950
    cmn r12, r3, lsl #1
    bhs Runtime_Unk_0202e974
}

asm void Runtime_Unk_0202e924(void)
{
    orrs r12, r3, r1
    bmi _14
    cmp r1, r3
    cmpeq r0, r2
    bx lr
_14:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    bx lr
}

asm void Runtime_Unk_0202e950(void)
{
    beq _c
    movs r12, #1
    bx lr
_c:
    cmp r0, #0
    bls _1c
    movs r12, #1
    bx lr
_1c:
    cmn r12, r3, lsl #1
    blo Runtime_Unk_0202e924
}

asm void Runtime_Unk_0202e974(void)
{
    beq _c
    movs r12, #1
    bx lr
_c:
    cmp r2, #0
    bls Runtime_Unk_0202e924
    movs r12, #1
    bx lr
}

asm void Runtime_Unk_0202e990(void)
{
    b Runtime_Unk_0202e924
}
