/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f4cbc();
extern u8 gGameState[];
s32 func_ov000_022f8c28();
s32 func_ov000_022f8bc8();
extern u8 gBossStatus[];
void BossFPartD8_State_Unk_ov000_022f3544();

void func_ov000_022f3588(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    func_ov000_022f4cbc(p0, 0);
    func_ov000_022f8c28(*(u32 *)gGameState, 0);
    func_ov000_022f8bc8(*(u32 *)gGameState, 0);
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x39;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_Unk_ov000_022f3544;
}
