/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gGameState[];
extern u8 gStageBGMHandleB[];
s32 NNS_SndPlayerMoveVolume();
s32 NNS_SndPlayerSetChannelPriority();
extern u8 gBGMVolume[];
extern u8 gStageBGMHandleA[];

void Stage_SelectBGMLayer(s32 p0) {
    if ((u32)*(u8 *)((u8 *)gGameState + 0x1c) >= (u32)9) {
        return;
    }
    if ((u32)*(u8 *)((u8 *)gGameState + 0x1d) >= (u32)2) {
        return;
    }
    if ((s32)p0 != 0) {
        NNS_SndPlayerMoveVolume(gStageBGMHandleB, 0, 0);
        NNS_SndPlayerSetChannelPriority(gStageBGMHandleB, 0);
        NNS_SndPlayerMoveVolume(gStageBGMHandleA, *(u8 *)gBGMVolume, 0);
        NNS_SndPlayerSetChannelPriority(gStageBGMHandleA, 0x40);
        return;
    }
    NNS_SndPlayerMoveVolume(gStageBGMHandleB, *(u8 *)gBGMVolume, 0);
    NNS_SndPlayerSetChannelPriority(gStageBGMHandleB, 0x40);
    NNS_SndPlayerMoveVolume(gStageBGMHandleA, 0, 0);
    NNS_SndPlayerSetChannelPriority(gStageBGMHandleA, 0);
}
