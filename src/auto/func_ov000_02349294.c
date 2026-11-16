/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_023475fc();
s32 func_ov000_0234a064();

void func_ov000_02349294(u8 *p0) {
    func_ov000_023475fc();
    if ((*(u32 *)((u8 *)p0 + 0x54) & 2) != 0) {
        return;
    }
    if ((*(u32 *)((u8 *)p0 + 0x58) & 1) == 0) {
        return;
    }
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x2aa) & ~0x100);
    *(u16 *)((u8 *)p0 + 0x2aa) = t0;
    func_ov000_0234a064(p0, 0, ((u8 *)p0 + 0x200), t0);
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x334))(p0, *(u32 *)((u8 *)p0 + 0x334));
}
