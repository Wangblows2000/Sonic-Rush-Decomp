/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 NNS_SndPlayerStopSeq();
s32 Sound_PlaySeqArcEx();
s32 EffZakoBomb_Create();
s32 EffZakoDebris_Create();
s32 ScreenShake_Start();
s32 ScreenFlash_Create();
s32 AttackHitbox_Create();

void func_ov000_02313bb0(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 0x20) != 0) {
        return;
    }
    NNS_SndPlayerStopSeq(*(u32 *)((u8 *)p0 + 0x1dc), 0);
    Sound_PlaySeqArcEx(0, 0, 0x64);
    EffZakoBomb_Create(p0, 0, 0);
    EffZakoDebris_Create(p0, 0, -24, 0, -0x200, 4);
    EffZakoDebris_Create(p0, 0, -24, 0, -0x200, 5);
    EffZakoDebris_Create(p0, 0, -24, 0, -0x200, 6);
    EffZakoDebris_Create(p0, 0, -24, 0, -0x200, 7);
    ScreenShake_Start(4);
    ScreenFlash_Create(0);
    s32 t0 = AttackHitbox_Create(p0, -48, -48, 0x30, 0x30, 8);
    if (t0 != 0) {
        *(u8 *)((u8 *)t0 + 0x188) = 0x21;
    }
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 8);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
}
