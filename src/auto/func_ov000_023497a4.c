/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020027a4();
s32 func_ov000_0234c4a4();
s32 Sound_PlaySeqArcEx();
s32 func_ov000_02345830();

void func_ov000_023497a4(u8 *p0) {
    s32 t0 = func_020027a4(*(s16 *)((u8 *)p0 + 0x30), *(s16 *)((u8 *)p0 + 0x2e));
    *(u8 *)((u8 *)p0 + 0x1) = (((s32)t0 >> 8) + 0x40);
    func_ov000_0234c4a4(p0, 0x38, (((s32)t0 >> 8) + 0x40));
    Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0xe);
    func_ov000_02345830(p0, 0x190);
}
