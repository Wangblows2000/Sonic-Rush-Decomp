/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f4ce4();
extern u8 gPlayers[];
s32 MapObject_Spawn();
s32 func_020366f4();
void func_ov000_022f10dc();

void func_ov000_022f112c(u8 *p0) {
    func_ov000_022f4ce4(p0, 0x2a, 0x1000, 1);
    MapObject_Spawn(0xde, *(u32 *)((u8 *)*(u32 *)gPlayers + 0x10), (*(u32 *)((u8 *)*(u32 *)gPlayers + 0x14) - 0x8000), ((u32)(*(u32 *)((u8 *)p0 + 0x458) << 0x10) >> 0x10), 0, 0, 0, 0, 0);
    func_020366f4(0, 0, 0xd1);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f10dc;
}
