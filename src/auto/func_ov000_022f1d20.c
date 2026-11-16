/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f46f4();
extern u8 data_ov000_02369b84[];
extern u8 gPlayers[];
s32 func_ov000_023385e4();
void func_ov000_022f238c();

void func_ov000_022f1d20(u8 *p0) {
    func_ov000_022f46f4();
    if ((*(u8 *)((u8 *)p0 + 0x57c) & 2) == 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_ov000_02369b84 + 0x21c);
    *(u32 *)((u8 *)*(u32 *)gPlayers + 0x70) = t0;
    s32 t1 = (*(u16 *)((u8 *)*(u32 *)gPlayers + 0x5c) | 0x40);
    s32 t2 = *(u32 *)gPlayers;
    *(u16 *)((u8 *)t2 + 0x5c) = t1;
    *(u32 *)((u8 *)p0 + 0x1b8) = (u32)p0;
    func_ov000_023385e4(t1, t2, t0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f238c;
}
