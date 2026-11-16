/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0201c070();

void func_ov000_022cee04(u8 *p0) {
    s32 t0 = (*(u32 *)p0 + 0x1000);
    s32 t1 = *(u32 *)((u8 *)(*(u32 *)p0 + 0x1000) + 0x420);
    if ((*(u8 *)((u8 *)*(u32 *)((u8 *)(*(u32 *)p0 + 0x1000) + 0x420) + 0x10c) & 2) == 0) {
        return;
    }
    if ((*(u8 *)((u8 *)t1 + 0x10f) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x4) = 0;
    *(u32 *)((u8 *)t0 + 0x420) = 0;
    func_0201c070(*(u32 *)((u8 *)(*(u32 *)p0 + 0x1000) + 0x454), 0, t0, t1);
}
