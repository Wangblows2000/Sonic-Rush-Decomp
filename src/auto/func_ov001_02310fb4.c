/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStagePlayer_SlowDownMap();
s32 SpecialStagePlayer_ChangeState();

void func_ov001_02310fb4(u8 *p0) {
    SpecialStagePlayer_SlowDownMap();
    if ((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) & 2) == 0) {
        return;
    }
    SpecialStagePlayer_ChangeState(p0, 0x19);
}
