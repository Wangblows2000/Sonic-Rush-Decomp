/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void RopeCLand_State_Idle(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0x88) == 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    *(u32 *)((u8 *)p0 + 0xc0) = (*(u32 *)((u8 *)p0 + 0xc0) & ~0x60000);
    *(u32 *)((u8 *)p0 + 0x108) = (*(u32 *)((u8 *)p0 + 0x108) & ~0x60000);
    if ((*(u32 *)((u8 *)t0 + 0x1f4) & 1) != 0) {
        *(u32 *)((u8 *)p0 + 0xc0) = (*(u32 *)((u8 *)p0 + 0xc0) | 0x20000);
        *(u32 *)((u8 *)p0 + 0x108) = (*(u32 *)((u8 *)p0 + 0x108) | 0x20000);
        return;
    }
    *(u32 *)((u8 *)p0 + 0xc0) = (*(u32 *)((u8 *)p0 + 0xc0) | 0x40000);
    *(u32 *)((u8 *)p0 + 0x108) = (*(u32 *)((u8 *)p0 + 0x108) | 0x40000);
}
