// asm: hand-written runtime library routine (split at mid-function entry points, see w_asm/split_plan.txt)
#include <nitro/types.h>

extern void _dcmpeq_e14(void);
extern void _dcmpeq_e40(void);
extern void _dcmpeq_e64(void);
void _dcmpeq(void);
void _dcmpeq_e80(void);

asm void _dcmpeq(void)
{
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs _dcmpeq_e40
    cmn r12, r3, lsl #1
    bhs _dcmpeq_e64
}

asm void _dcmpeq_e14(void)
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

asm void _dcmpeq_e40(void)
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
    blo _dcmpeq_e14
}

asm void _dcmpeq_e64(void)
{
    beq _c
    movs r12, #1
    bx lr
_c:
    cmp r2, #0
    bls _dcmpeq_e14
    movs r12, #1
    bx lr
}

asm void _dcmpeq_e80(void)
{
    b _dcmpeq_e14
}
