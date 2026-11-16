/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBGMVolume[];
extern u8 gStageBGMHandleA[];
s32 NNS_SndPlayerMoveVolume();
extern u8 gStageBGMHandleB[];
s32 NNS_SndPlayerSetChannelPriority();

void Stage_FadeBGMLayer(s32 p0, s32 p1, s32 p2) {
    if ((s32)p0 != 0) {
        s32 t0 = ((s32)((s32)p0 * *(u8 *)gBGMVolume) >> 7);
        if ((s32)p2 != 0) {
            NNS_SndPlayerMoveVolume(gStageBGMHandleA, t0, p1);
            NNS_SndPlayerSetChannelPriority(gStageBGMHandleB, 0);
            NNS_SndPlayerSetChannelPriority(gStageBGMHandleA, 0x40);
            return;
        }
        NNS_SndPlayerMoveVolume(gStageBGMHandleB, t0, p1);
        NNS_SndPlayerSetChannelPriority(gStageBGMHandleB, 0x40);
        NNS_SndPlayerSetChannelPriority(gStageBGMHandleA, 0);
        return;
    }
    s32 t1 = ((s32)((s32)p0 * *(u8 *)gBGMVolume) >> 7);
    NNS_SndPlayerMoveVolume(gStageBGMHandleB, ((s32)((s32)p0 * *(u8 *)gBGMVolume) >> 7), p1);
    NNS_SndPlayerMoveVolume(gStageBGMHandleA, t1, p1);
}
