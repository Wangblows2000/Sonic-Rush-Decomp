/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
s32 func_ov000_0234c3b4();
extern u8 data_022c4534[];
s32 func_ov000_022f1aac();

void BossFPartD8_State_Unk_ov000_022efa2c(s32 p0) {
    *(u16 *)((u8 *)p0 + 0x434) = (*(u16 *)((u8 *)p0 + 0x434) + 0x1);
    if ((u32)*(u16 *)((u8 *)p0 + 0x434) <= (u32)0x3c) {
        return;
    }
    s32 t0 = *(u32 *)gPlayers;
    if ((*(u32 *)((u8 *)*(u32 *)gPlayers + 0x32c) & 0x400) == 0) {
        s32 t1 = (*(u32 *)((u8 *)t0 + 0x58) | 0x80);
        *(u32 *)((u8 *)t0 + 0x58) = t1;
        func_ov000_0234c3b4(*(u32 *)gPlayers, t1, t0);
    }
    *(u32 *)((u8 *)data_022c4534 + 0x24) = 0x340;
    func_ov000_022f1aac(p0, data_022c4534, 0x340);
}
