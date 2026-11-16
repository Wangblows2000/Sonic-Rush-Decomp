/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f4cbc();
extern u8 data_022c4560[];
s32 func_ov000_022f8bc8();
extern u8 data_ov000_02369ac0[];
void BossFPartD8_State_Unk_ov000_022f2524();

void func_ov000_022f2550(u8 *p0) {
    func_ov000_022f4cbc();
    func_ov000_022f8bc8(*(u32 *)data_022c4560, 0);
    *(u32 *)((u8 *)data_ov000_02369ac0 + 0x4) = 0x39;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_Unk_ov000_022f2524;
}
