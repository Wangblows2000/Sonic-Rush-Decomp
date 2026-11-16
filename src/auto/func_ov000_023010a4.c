/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void Kuragen_State_Unk_ov000_02301034();

void func_ov000_023010a4(u8 *p0) {
    StageObj_SetAnimation(p0, 1);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    if (t0 == 0) {
        *(u32 *)((u8 *)p0 + 0x10) = 0;
        *(u32 *)((u8 *)p0 + 0x14) = 0xf00;
    } else {
        *(u32 *)((u8 *)p0 + 0x10) = *(u32 *)((u8 *)t0 + 0x10);
        *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x14) + 0xf00);
    }
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Kuragen_State_Unk_ov000_02301034;
}
