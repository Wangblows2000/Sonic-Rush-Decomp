/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 NNS_SndPlayerStopSeq();
s32 NNS_SndHandleReleaseSeq();

void Player_StopBoost(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x80) == 0) {
        return;
    }
    if (*(s16 *)((u8 *)p0 + 0x3b2) == 0) {
        s32 t0 = *(s16 *)((u8 *)p0 + 0x3b4);
        if (t0 == 0) {
            *(u32 *)((u8 *)p0 + 0x2c4) = 2;
        }
    }
    *(u8 *)((u8 *)p0 + 0x3ce) = 8;
    NNS_SndPlayerStopSeq(((u8 *)p0 + 0x38c), 0x10, 8);
    NNS_SndHandleReleaseSeq(((u8 *)p0 + 0x38c));
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0x80);
}
