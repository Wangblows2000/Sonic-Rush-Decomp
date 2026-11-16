/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void GmkLand_State_Unk_ov000_0230f280();
void GmkLand_Draw();
s32 Sound_PlaySeqArcEx();
extern u8 gPlayers[];
s32 Sound_SetPositional();

void GmkLand_Action_Unk_ov000_0230f474(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x68) = (u32)GmkLand_State_Unk_ov000_0230f280;
    *(u32 *)((u8 *)p0 + 0x70) = (u32)GmkLand_Draw;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x4b);
    Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)gPlayers);
}
