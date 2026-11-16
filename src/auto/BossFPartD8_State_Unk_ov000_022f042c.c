/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossF_UpdateCameraShake();
s32 BossFPartD8_Unk_ov000_022f0fdc();
void BossFPartD8_State_Unk_ov000_022f03b4();

void BossFPartD8_State_Unk_ov000_022f042c(u8 *p0) {
    BossF_UpdateCameraShake(((u8 *)p0 + 0x450), 4, 8);
    BossFPartD8_Unk_ov000_022f0fdc(p0, 8);
    s32 t0 = *(u16 *)((u8 *)p0 + 0x454);
    *(u16 *)((u8 *)p0 + 0x454) = (*(u16 *)((u8 *)p0 + 0x454) + 0x1);
    if ((u32)t0 > (u32)0x1e) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_Unk_ov000_022f03b4;
    }
}
