/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
extern u8 data_022c44f4[];
s32 NNS_SndPlayerMoveVolume();
s32 NNS_SndPlayerSetChannelPriority();
extern u8 gBGMVolume[];
extern u8 data_022c44f0[];

void func_0204c66c(s32 p0) {
    if ((u32)*(u8 *)((u8 *)data_022c4560 + 0x1c) >= (u32)9) {
        return;
    }
    if ((u32)*(u8 *)((u8 *)data_022c4560 + 0x1d) >= (u32)2) {
        return;
    }
    if ((s32)p0 != 0) {
        NNS_SndPlayerMoveVolume(data_022c44f4, 0, 0);
        NNS_SndPlayerSetChannelPriority(data_022c44f4, 0);
        NNS_SndPlayerMoveVolume(data_022c44f0, *(u8 *)gBGMVolume, 0);
        NNS_SndPlayerSetChannelPriority(data_022c44f0, 0x40);
        return;
    }
    NNS_SndPlayerMoveVolume(data_022c44f4, *(u8 *)gBGMVolume, 0);
    NNS_SndPlayerSetChannelPriority(data_022c44f4, 0x40);
    NNS_SndPlayerMoveVolume(data_022c44f0, 0, 0);
    NNS_SndPlayerSetChannelPriority(data_022c44f0, 0);
}
