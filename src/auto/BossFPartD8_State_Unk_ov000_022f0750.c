/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_SetAnimation();
extern u8 data_ov000_0236a444[];
void BossFPartD8_State_Unk_ov000_022f065c();

void BossFPartD8_State_Unk_ov000_022f0750(u8 *p0) {
    BossFPartD8_SetAnimation(p0, 0x20, 0x1000, 0);
    *(u32 *)((u8 *)p0 + 0x440) = (*(u32 *)((u8 *)p0 + 0x10) << 4);
    *(u32 *)((u8 *)p0 + 0x444) = (*(u32 *)((u8 *)p0 + 0x14) << 4);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x18) << 4);
    *(u32 *)((u8 *)p0 + 0x448) = t0;
    *(u16 *)((u8 *)p0 + 0x44c) = 0;
    *(u32 *)((u8 *)data_ov000_0236a444 + 0xc0) = 0x258000;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_Unk_ov000_022f065c;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x68))(p0, *(u32 *)((u8 *)p0 + 0x68), 0x258000, t0);
}
