/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
s32 func_ov000_02330aec();

void func_ov000_02342f34(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x20000000) == 0) {
        Sound_PlaySeqArcEx(0, 0, 0x35);
        func_ov000_02330aec(p0);
    }
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x30000000);
}
