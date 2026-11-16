/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
s32 func_ov000_0232e2f0();
s32 func_ov000_0232dedc();
s32 ScreenShake_Start();
s32 ScreenFlash_Create();

void func_ov000_023155cc(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 0x20) != 0) {
        return;
    }
    Sound_PlaySeqArcEx(0, 0, 0x64);
    func_ov000_0232e2f0(p0, 0, 0);
    func_ov000_0232dedc(p0, 0, -24, 0, -0x200, 4);
    func_ov000_0232dedc(p0, 0, -24, 0, -0x200, 5);
    func_ov000_0232dedc(p0, 0, -24, 0, -0x200, 6);
    func_ov000_0232dedc(p0, 0, -24, 0, -0x200, 7);
    ScreenShake_Start(4);
    ScreenFlash_Create(0);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 8);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
}
