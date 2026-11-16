/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossF_UpdateCameraShake();
s32 BossFPartD8_Unk_ov000_022f3f8c();
s32 BossFPartD8_Unk_ov000_022f88dc();

void BossFPartD8_State_Unk_ov000_022f3504(u8 *p0) {
    BossF_UpdateCameraShake(((u8 *)p0 + 0x43a), 2, 6);
    if ((*(u8 *)((u8 *)p0 + 0x57c) & 2) == 0) {
        return;
    }
    BossFPartD8_Unk_ov000_022f3f8c((*(u8 *)((u8 *)p0 + 0x57c) & 2));
    BossFPartD8_Unk_ov000_022f88dc();
}
