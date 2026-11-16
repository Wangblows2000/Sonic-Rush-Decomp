/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_023425f4();
extern u8 data_ov000_023643f0[];

void MapObj73_State_Unk_ov000_02303dec(u8 *p0) {
    func_ov000_023425f4(*(u32 *)((u8 *)p0 + 0x10), *(u32 *)((u8 *)p0 + 0x14));
    ((s32 (*)())*(u32 *)((u8 *)data_ov000_023643f0 + ((*(u32 *)((u8 *)p0 + 0x218) & 0xff) << 2)))(p0, *(u32 *)((u8 *)data_ov000_023643f0 + ((*(u32 *)((u8 *)p0 + 0x218) & 0xff) << 2)));
}
