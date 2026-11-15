/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0233d538();
s32 func_ov000_0233d0fc();
s32 func_ov000_0233cb5c();

void func_ov000_0233d7ec(u8 *p0) {
    func_ov000_0233d538();
    func_ov000_0233d0fc(p0, 0);
    func_ov000_0233d0fc(p0, 1);
    func_ov000_0233cb5c(p0);
    s32 t0 = (*(u32 *)p0 | 0x80000000);
    *(u32 *)p0 = t0;
}
