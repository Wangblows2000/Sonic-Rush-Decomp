/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_0207584c[];
s32 Sound_PlayVoice();

void PlayerDemoOp_PlayVoice(u8 *p0, s32 p1) {
    if ((*(u32 *)((u8 *)p0 + 0x4) & 0x10000000) == 0) {
        Sound_PlayVoice(0, *(u32 *)((u8 *)*(u32 *)*(u32 *)((u8 *)p0 + 0x2c) + 0x4), *(s16 *)((u8 *)data_0207584c + ((s32)p1 << 1)));
    }
}

#pragma thumb reset
