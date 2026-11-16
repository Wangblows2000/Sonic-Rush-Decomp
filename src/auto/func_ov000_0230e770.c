/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void JetCatapult_State_Unk_ov000_0230e6e8();
s32 ScreenShake_Start();
s32 Sound_PlaySeqArcEx();

void func_ov000_0230e770(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x68) = (u32)JetCatapult_State_Unk_ov000_0230e6e8;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x100);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x40);
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x5c) | 0x100);
    *(u16 *)((u8 *)p0 + 0x5c) = t0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u16 *)((u8 *)p0 + 0x34) = 0xa00;
    ScreenShake_Start(4, 0xa00, 0, t0);
    *(u16 *)((u8 *)p0 + 0x62) = 0x80;
    Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x6c);
}
