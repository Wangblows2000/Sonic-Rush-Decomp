// asm: hand-written runtime library routine (split at mid-function entry points, see w_asm/split_plan.txt)
#include <nitro/types.h>

extern void Runtime_Unk_0202ec60(void);
extern void Runtime_Unk_0202ec90(void);
extern void Runtime_Unk_0202ecbc(void);
void Runtime_Unk_0202ec4c(void);
void Runtime_Unk_0202ece0(void);

asm void Runtime_Unk_0202ec4c(void)
{
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs Runtime_Unk_0202ec90
    cmn r12, r3, lsl #1
    bhs Runtime_Unk_0202ecbc
}

asm void Runtime_Unk_0202ec60(void)
{
    orrs r12, r3, r1
    bmi _14
    cmp r1, r3
    cmpeq r0, r2
    bx lr
_14:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    cmpeq r1, r1
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    bx lr
}

asm void Runtime_Unk_0202ec90(void)
{
    beq _10
    mov r12, #0
    adds r12, r12, #0
    bx lr
_10:
    cmp r0, #0
    bls _24
    mov r12, #0
    adds r12, r12, #0
    bx lr
_24:
    cmn r12, r3, lsl #1
    blo Runtime_Unk_0202ec60
}

asm void Runtime_Unk_0202ecbc(void)
{
    beq _10
    mov r12, #0
    adds r12, r12, #0
    bx lr
_10:
    cmp r2, #0
    bls Runtime_Unk_0202ec60
    mov r12, #0
    adds r12, r12, #0
    bx lr
}

asm void Runtime_Unk_0202ece0(void)
{
    b Runtime_Unk_0202ec60
}
