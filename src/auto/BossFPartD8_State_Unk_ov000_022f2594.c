/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f8fa0();
s32 func_ov000_022f8888();

void BossFPartD8_State_Unk_ov000_022f2594(u8 *p0) {
    func_ov000_022f8fa0(((u8 *)p0 + 0x436), 2, 6);
    if ((*(u8 *)((u8 *)p0 + 0x57c) & 2) == 0) {
        return;
    }
    func_ov000_022f8888((*(u8 *)((u8 *)p0 + 0x57c) & 2));
}
