/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossF_UpdateCameraShake();
extern u8 gGameState[];
s32 BossFPartD8_Unk_ov000_022f8c28();
s32 BossFPartD8_Unk_ov000_022f8bc8();
s32 BossFPartD8_Unk_ov000_022f4cbc();
void BossFPartD8_State_Unk_ov000_022f2ee0();

void BossFPartD8_State_Unk_ov000_022f2f0c(u8 *p0) {
    BossF_UpdateCameraShake(((u8 *)p0 + 0x436), 3, 7);
    *(u16 *)((u8 *)p0 + 0x43e) = (*(u16 *)((u8 *)p0 + 0x43e) + 0x1);
    if ((*(u16 *)((u8 *)p0 + 0x43e) & 3) == 0) {
        if ((*(u16 *)((u8 *)p0 + 0x43a) & 0x8000) != 0) {
            BossFPartD8_Unk_ov000_022f8c28(*(u32 *)gGameState, 0);
            BossFPartD8_Unk_ov000_022f8bc8(*(u32 *)gGameState, 0);
        } else {
            BossFPartD8_Unk_ov000_022f8c28(*(u32 *)gGameState, 1);
            BossFPartD8_Unk_ov000_022f8bc8(*(u32 *)gGameState, 1);
        }
        *(u16 *)((u8 *)p0 + 0x43a) = (*(u16 *)((u8 *)p0 + 0x43a) & ~3);
        *(u16 *)((u8 *)p0 + 0x43a) = (*(u16 *)((u8 *)p0 + 0x43a) ^ 0x8000);
    }
    s32 t0 = *(u16 *)((u8 *)p0 + 0x438);
    if (t0 != 0) {
        *(u16 *)((u8 *)p0 + 0x438) = (t0 - 0x1);
    }
    if (*(u16 *)((u8 *)p0 + 0x438) != 0) {
        return;
    }
    if ((*(u16 *)((u8 *)p0 + 0x43a) & 0x8000) != 0) {
        return;
    }
    BossFPartD8_Unk_ov000_022f4cbc(p0, *(u16 *)((u8 *)p0 + 0x438));
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_Unk_ov000_022f2ee0;
}
