/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
s32 func_ov000_022f8c88();
extern u8 gPlayers[];
void func_ov000_022efaa0();

void BossFPartD8_State_Unk_ov000_022efb08(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x434) = (*(u16 *)((u8 *)p0 + 0x434) + 0x1);
    s32 t0 = *(u16 *)((u8 *)p0 + 0x434);
    if ((*(u16 *)((u8 *)p0 + 0x434) & 1) == 0) {
        if ((t0 & 0x8000) != 0) {
            func_ov000_022f8c88(*(u32 *)data_022c4560, 0);
        } else {
            func_ov000_022f8c88(*(u32 *)data_022c4560, 1);
        }
        *(u16 *)((u8 *)p0 + 0x434) = (*(u16 *)((u8 *)p0 + 0x434) & ~1);
        *(u16 *)((u8 *)p0 + 0x434) = (*(u16 *)((u8 *)p0 + 0x434) ^ 0x8000);
    }
    s32 t1 = *(u32 *)gPlayers;
    if ((*(u32 *)((u8 *)*(u32 *)gPlayers + 0x32c) & 0x400) == 0) {
        s32 t2 = *(u32 *)((u8 *)t1 + 0x14);
        if (t2 > -0x6400) {
            *(u32 *)((u8 *)t1 + 0x14) = (t2 - 0x800);
        }
    }
    if ((*(u8 *)((u8 *)p0 + 0x57c) & 2) == 0) {
        return;
    }
    s32 t3 = (*(u16 *)((u8 *)p0 + 0x434) & 0x8000);
    if (t3 == 0) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022efaa0;
    }
}
