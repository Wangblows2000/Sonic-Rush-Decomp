/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
s32 func_0206c95c();
extern u8 gPlayers[];
s32 func_0206c938();
extern u8 data_ov000_0236aadc[];
extern u8 data_ov000_0236aae0[];
s32 func_ov000_0233b1b8();
s32 func_ov000_0233b198();

void func_ov000_02342834(s32 p0) {
    if ((*(u32 *)((u8 *)data_022c4560 + 0x18) & 0x20) == 0) {
        return;
    }
    if ((s32)p0 == 0) {
        return;
    }
    func_0206c95c((*(u32 *)((u8 *)data_022c4560 + 0x18) & 0x20), data_022c4560);
    s32 t0 = func_0206c938(1, *(u8 *)((u8 *)*(u32 *)((u8 *)gPlayers + ((s32)p0 << 2)) + 0x326));
    if (t0 == 0) {
        return;
    }
    s32 t1 = *(u32 *)((u8 *)gPlayers + ((s32)p0 << 2));
    *(u32 *)((u8 *)data_ov000_0236aadc + (*(u8 *)((u8 *)*(u32 *)((u8 *)gPlayers + ((s32)p0 << 2)) + 0x327) * 0x64)) = *(u32 *)((u8 *)t0 + 0x24);
    s32 t2 = (*(u8 *)((u8 *)t1 + 0x327) * 0x64);
    s32 t3 = *(u8 *)((u8 *)t1 + 0x327);
    *(u32 *)((u8 *)data_ov000_0236aae0 + t2) = *(u32 *)((u8 *)t0 + 0x28);
    func_ov000_0233b1b8(p0, t2, data_ov000_0236aae0, t3);
    func_ov000_0233b198(p0);
}
