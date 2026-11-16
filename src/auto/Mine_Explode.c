/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
s32 func_ov000_0232e2f0();
s32 EffZakoDebris_Create();
s32 ScreenShake_Start();
s32 ScreenFlash_Create();
s32 func_ov000_02343498();

void Mine_Explode(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x54) & 2) != 0) {
        return;
    }
    Sound_PlaySeqArcEx(0, 0, 0x59);
    func_ov000_0232e2f0(p0, 0, 0);
    EffZakoDebris_Create(p0, 0, -24, 0, -0x200, 4);
    EffZakoDebris_Create(p0, 0, -24, 0, -0x200, 5);
    EffZakoDebris_Create(p0, 0, -24, 0, -0x200, 6);
    EffZakoDebris_Create(p0, 0, -24, 0, -0x200, 7);
    ScreenShake_Start(4);
    ScreenFlash_Create(0);
    s32 t0 = func_ov000_02343498(p0, -48, -48, 0x30, 0x30, 8);
    if (t0 != 0) {
        *(u8 *)((u8 *)t0 + 0x188) = (*(u8 *)((u8 *)t0 + 0x188) | 0x20);
    }
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 2);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 0x1000);
}
