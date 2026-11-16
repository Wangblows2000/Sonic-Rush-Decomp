/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_DisableHitboxes();
extern u8 data_ov000_02363e48[];
void BossFPartD8_State_HandSlamStrike();
s32 BossFPartD8_SetAnimation();
void BossFPartD8_State_HandSlamLiftWait();

void BossFPartD8_State_HandSlamLift(u8 *p0) {
    BossFPartD8_DisableHitboxes();
    *(u16 *)((u8 *)p0 + 0x438) = (*(u16 *)((u8 *)p0 + 0x438) + 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x458);
    s32 v1 = (s32)data_ov000_02363e48;
    if (*(u16 *)((u8 *)p0 + 0x438) < *(u8 *)((u8 *)data_ov000_02363e48 + *(u32 *)((u8 *)p0 + 0x458))) {
        if (t0 >= 6) {
            *(u32 *)((u8 *)p0 + 0x434) = (*(u32 *)((u8 *)p0 + 0x434) == 0);
            *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_HandSlamStrike;
            return;
        }
        v1 = ((*(u32 *)((u8 *)p0 + 0x434) != 0) ? 0xe : 0xf);
    } else {
        v1 = ((*(u32 *)((u8 *)p0 + 0x434) != 0) ? 0xc : 0xd);
    }
    BossFPartD8_SetAnimation(p0, v1, 0x1000, 0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_HandSlamLiftWait;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x68))(p0, *(u32 *)((u8 *)p0 + 0x68));
}
