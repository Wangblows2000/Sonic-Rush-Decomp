/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void Kuragen_State_Unk_ov000_02300f78();

void Kuragen_Action_Unk_ov000_02300fd0(u8 *p0) {
    StageObj_SetAnimation(p0, 1);
    *(u16 *)((u8 *)p0 + 0x30) = 0x280;
    s32 t0 = (*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x4) & 0xc);
    if (t0 != 0) {
        *(u16 *)((u8 *)p0 + 0x2e) = ((s32)(*(s16 *)((u8 *)p0 + 0x2e) * ((((s32)t0 >> 2) << 7) + 0x100)) >> 8);
    }
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Kuragen_State_Unk_ov000_02300f78;
}
