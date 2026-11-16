/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
s32 func_ov000_02342a74();
s32 func_ov000_02330774();

void func_ov000_02342fb4(s32 p0) {
    Sound_PlaySeqArcEx(0, 0, 0x3d);
    func_ov000_02342a74(p0, 0x16);
    *(u16 *)((u8 *)p0 + 0x3ca) = 0;
    *(u16 *)((u8 *)p0 + 0x3b4) = 0x3e7;
    func_ov000_02330774(p0, ((u8 *)p0 + 0x300), 0x3e7);
}
