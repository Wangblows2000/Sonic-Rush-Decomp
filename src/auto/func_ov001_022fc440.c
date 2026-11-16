/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_022fa558();
void func_ov001_022fc34c();

void func_ov001_022fc440(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x84);
    func_ov001_022fa558();
    if ((*(u8 *)((u8 *)t0 + 0x10c) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x74) = (u32)func_ov001_022fc34c;
    *(u32 *)((u8 *)p0 + 0x1cc) = 0x3c;
    *(u32 *)((u8 *)p0 + 0xf0) = (*(u32 *)((u8 *)p0 + 0xf0) & ~0x200);
}
