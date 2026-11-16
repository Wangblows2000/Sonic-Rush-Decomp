/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369ac0[];
extern u8 data_ov000_023635c0[];
s32 MapObject_Spawn();

void func_ov000_022dc858(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0xbe4) = (*(u32 *)((u8 *)p0 + 0xbe4) - 0x1);
    *(u32 *)((u8 *)p0 + 0xbe0) = 1;
    *(u32 *)((u8 *)p0 + 0xbe8) = *(s16 *)((u8 *)data_ov000_023635c0 + ((*(u16 *)((u8 *)data_ov000_02369ac0 + 0x8) - 0x1) << 1));
    MapObject_Spawn(0xd0, 0, 0, 0, 0, 0, 0, 0, 0);
}
