/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArc();
void func_ov000_022f569c();

void func_ov000_022f57f8(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x4a4) = 0;
    *(u16 *)((u8 *)p0 + 0x13e) = (*(u16 *)((u8 *)p0 + 0x13e) | 4);
    Sound_PlaySeqArc(((u8 *)p0 + 0x4a8), 0, 0xe3, ((u8 *)p0 + 0x100));
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f569c;
}
