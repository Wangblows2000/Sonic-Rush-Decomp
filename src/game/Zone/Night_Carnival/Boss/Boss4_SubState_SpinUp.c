/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Boss4_GetHealthSpinScale();
s32 _fmul();
s32 _ffix();
s32 Boss4_Action_StartSpin();

void Boss4_SubState_SpinUp(u8 *p0) {
    s32 t0 = Boss4_GetHealthSpinScale();
    _fmul(0x42c00000, t0);
    s32 t1 = _ffix();
    *(u32 *)((u8 *)p0 + 0xbc0) = (*(u32 *)((u8 *)p0 + 0xbc0) + 0x8);
    s32 t2 = *(u32 *)((u8 *)p0 + 0xbc0);
    if (t2 < t1) {
        s32 t3 = (*(u32 *)((u8 *)p0 + 0xbb8) + t2);
        *(u32 *)((u8 *)p0 + 0xbb8) = t3;
        return;
    }
    *(u32 *)((u8 *)p0 + 0xbc0) = t1;
    Boss4_Action_StartSpin(p0, t2);
}
