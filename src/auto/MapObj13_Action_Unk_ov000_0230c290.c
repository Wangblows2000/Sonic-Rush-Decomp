/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void MapObj13_State_Unk_ov000_0230c1c8();

void MapObj13_Action_Unk_ov000_0230c290(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x68) = (u32)MapObj13_State_Unk_ov000_0230c1c8;
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x20);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x8000000);
    *(u16 *)((u8 *)p0 + 0x2e) = (*(s8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x6) << 6);
    *(u16 *)((u8 *)p0 + 0x32) = 0x400;
}
