/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossF_UpdateCameraShake();
void BossFPartD8_State_PartDAThrowWindup();

void BossFPartD8_State_PartDAThrowDelay(u8 *p0) {
    BossF_UpdateCameraShake(((u8 *)p0 + 0x436), 2, 6);
    s32 t0 = *(u16 *)((u8 *)p0 + 0x438);
    *(u16 *)((u8 *)p0 + 0x438) = (*(u16 *)((u8 *)p0 + 0x438) - 0x1);
    if (t0 == 0) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_PartDAThrowWindup;
    }
}
