/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBGMVolume[];
extern u8 data_022c44f0[];
s32 NNS_SndPlayerMoveVolume();
extern u8 data_022c44f4[];
s32 NNS_SndPlayerSetChannelPriority();

void func_0204c598(s32 p0, s32 p1, s32 p2) {
    if ((s32)p0 != 0) {
        s32 t0 = ((s32)((s32)p0 * *(u8 *)gBGMVolume) >> 7);
        if ((s32)p2 != 0) {
            NNS_SndPlayerMoveVolume(data_022c44f0, t0, p1);
            NNS_SndPlayerSetChannelPriority(data_022c44f4, 0);
            NNS_SndPlayerSetChannelPriority(data_022c44f0, 0x40);
            return;
        }
        NNS_SndPlayerMoveVolume(data_022c44f4, t0, p1);
        NNS_SndPlayerSetChannelPriority(data_022c44f4, 0x40);
        NNS_SndPlayerSetChannelPriority(data_022c44f0, 0);
        return;
    }
    s32 t1 = ((s32)((s32)p0 * *(u8 *)gBGMVolume) >> 7);
    NNS_SndPlayerMoveVolume(data_022c44f4, ((s32)((s32)p0 * *(u8 *)gBGMVolume) >> 7), p1);
    NNS_SndPlayerMoveVolume(data_022c44f0, t1, p1);
}
