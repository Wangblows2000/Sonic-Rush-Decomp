/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_0230d6f4();
s32 func_ov001_0231599c();
s32 Sound_PlaySeqArc();

void func_ov001_023120ec(u8 *p0, u8 *p1) {
    func_ov001_0230d6f4(0x6000);
    *(u32 *)((u8 *)p0 + 0x144) = 0x3c;
    func_ov001_0231599c(3);
    *(u16 *)((u8 *)p0 + 0x16) = *(u16 *)((u8 *)p1 + 0x16);
    Sound_PlaySeqArc(0, 0x10, 0x17);
}
