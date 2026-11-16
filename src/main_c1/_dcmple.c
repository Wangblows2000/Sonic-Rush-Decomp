// asm: hand-written runtime library routine (split at mid-function entry points, see w_asm/split_plan.txt)
#include <nitro/types.h>

extern void _dcmple_e14(void);
extern void _dcmple_e44(void);
extern void _dcmple_e70(void);
void _dcmple(void);
void _dcmple_e94(void);

asm void _dcmple(void)
{
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs _dcmple_e44
    cmn r12, r3, lsl #1
    bhs _dcmple_e70
}

asm void _dcmple_e14(void)
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

asm void _dcmple_e44(void)
{
    beq _10
    mov r12, #0xff000000
    adds r12, r12, #0x10000000
    bx lr
_10:
    cmp r0, #0
    bls _24
    mov r12, #0xff000000
    adds r12, r12, #0x10000000
    bx lr
_24:
    cmn r12, r3, lsl #1
    blo _dcmple_e14
}

asm void _dcmple_e70(void)
{
    beq _10
    mov r12, #0xff000000
    adds r12, r12, #0x10000000
    bx lr
_10:
    cmp r2, #0
    bls _dcmple_e14
    mov r12, #0xff000000
    adds r12, r12, #0x10000000
    bx lr
}

asm void _dcmple_e94(void)
{
    b _dcmple_e14
}
