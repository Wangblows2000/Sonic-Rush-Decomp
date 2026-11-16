/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0234a064();

void Player_State_Saucer(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0x3e8) != 0) {
        if ((*(u16 *)((u8 *)p0 + 0x422) & 3) == 0) {
            return;
        }
    }
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u32 *)((u8 *)p0 + 0x3e8) = 0;
    s32 t0 = *(u32 *)((u8 *)p0 + 0x32c);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & 0xffefefff);
    *(u8 *)((u8 *)p0 + 0x1) = 0;
    if ((*(u16 *)((u8 *)p0 + 0x422) & 3) != 0) {
        ((s32 (*)())*(u32 *)((u8 *)p0 + 0x340))(p0, *(u32 *)((u8 *)p0 + 0x340), ((u8 *)p0 + 0x400), t0);
        return;
    }
    func_ov000_0234a064(p0);
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x334))(p0, *(u32 *)((u8 *)p0 + 0x334));
}
