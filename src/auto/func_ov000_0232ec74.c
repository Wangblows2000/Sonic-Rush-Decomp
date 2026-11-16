/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0201c070();
s32 func_0201bf1c();
s32 func_020365bc();

void func_ov000_0232ec74(u8 *p0) {
    if (*(u32 *)*(u32 *)((u8 *)p0 + 0x10) == 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_0201c070(*(u32 *)*(u32 *)((u8 *)p0 + 0x10), 0);
    func_0201bf1c(*(u32 *)t0);
    func_020365bc(*(u32 *)t0);
}
