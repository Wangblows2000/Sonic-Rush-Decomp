/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_02302398();
s32 ExtraBossPlayer_ChangeState();

void func_ov001_02301cc8(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x84);
    func_ov001_02302398();
    if ((*(u8 *)((u8 *)t0 + 0x10c) & 2) == 0) {
        return;
    }
    ExtraBossPlayer_ChangeState(p0, 9);
    *(u32 *)((u8 *)p0 + 0x1c8) = 0x3d;
}
