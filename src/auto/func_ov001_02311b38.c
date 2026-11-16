/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_02312138();
s32 SpecialStagePlayer_ChangeState();

void func_ov001_02311b38(u8 *p0) {
    s32 t0 = func_ov001_02312138();
    if (t0 == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x18) = 0;
    *(u32 *)((u8 *)p0 + 0x48) = 0;
    *(u32 *)((u8 *)p0 + 0x24) = 0;
    SpecialStagePlayer_ChangeState(p0, 7, 0);
}
