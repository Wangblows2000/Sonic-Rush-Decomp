/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
extern u8 gPlayers[];
s32 Sound_SetPositional();
s32 func_ov000_0232e2f0();
s32 EffZakoDebris_Create();
s32 ScreenShake_Start();
s32 ScreenFlash_Create();
s32 func_ov000_02343498();

void func_ov000_022ff668(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 0x20) != 0) {
        return;
    }
    Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x59);
    Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)gPlayers);
    func_ov000_0232e2f0(p0, 0, 0);
    EffZakoDebris_Create(p0, 0, -24, 0, -0x200, 4);
    EffZakoDebris_Create(p0, 0, -24, 0, -0x200, 5);
    ScreenShake_Start(3);
    ScreenFlash_Create(0);
    func_ov000_02343498(p0, -24, -24, 0x18, 0x18, 8);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 8);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x100);
}
