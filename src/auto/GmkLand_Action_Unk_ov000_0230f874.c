/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void GmkLand_State_Unk_ov000_0230f7b0();

void GmkLand_Action_Unk_ov000_0230f874(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x2);
    if (t0 == 0x53) {
        StageObj_SetAnimation(p0, 2);
    } else {
        if (t0 == 0x52) {
            StageObj_SetAnimation(p0, 1);
        } else {
            StageObj_SetAnimation(p0, 0);
        }
    }
    *(u32 *)((u8 *)p0 + 0x68) = (u32)GmkLand_State_Unk_ov000_0230f7b0;
}
