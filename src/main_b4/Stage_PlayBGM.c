#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x18];
    u8 unk1c;
    u8 unk1d;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
extern u8 gStageBGMHandleB[];
extern u8 gStageBGMHandleA[];
extern void *gBGMSoundHandle;
extern u16 data_02074850[][3][2];
extern u16 data_02074852[][3][2];
extern u8 gBGMVolume;

void NNS_SndHandleInit(void *p);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void Sound_PlaySeq(void *p, u16 a);
void NNS_SndArcPlayerStartSeq(void *p, u16 a);
void NNS_SndPlayerSetVolume(void *p, u8 a);
void Stage_SelectBGMLayer(s32 a);

void Stage_PlayBGM(s32 a) {
    NNS_SndHandleInit(gStageBGMHandleB);
    NNS_SndHandleInit(gStageBGMHandleA);
    if (gGameState.unk1c >= 9 || gGameState.unk1d >= 2) {
        NNS_SndPlayerStopSeq(&gBGMSoundHandle, 0);
        Sound_PlaySeq(&gBGMSoundHandle, data_02074850[gGameState.unk1c][gGameState.unk1d][0]);
        return;
    }
    NNS_SndPlayerStopSeq(gStageBGMHandleB, 0);
    NNS_SndPlayerStopSeq(gStageBGMHandleA, 0);
    if (gGameState.unk0 == 0) {
        NNS_SndArcPlayerStartSeq(gStageBGMHandleB, data_02074850[gGameState.unk1c][gGameState.unk1d][0]);
        NNS_SndPlayerSetVolume(gStageBGMHandleB, gBGMVolume);
        NNS_SndArcPlayerStartSeq(gStageBGMHandleA, data_02074852[gGameState.unk1c][gGameState.unk1d][0]);
        NNS_SndPlayerSetVolume(gStageBGMHandleA, gBGMVolume);
    } else {
        NNS_SndArcPlayerStartSeq(gStageBGMHandleB, data_02074852[gGameState.unk1c][gGameState.unk1d][0]);
        NNS_SndPlayerSetVolume(gStageBGMHandleB, gBGMVolume);
        NNS_SndArcPlayerStartSeq(gStageBGMHandleA, data_02074850[gGameState.unk1c][gGameState.unk1d][0]);
        NNS_SndPlayerSetVolume(gStageBGMHandleA, gBGMVolume);
    }
    Stage_SelectBGMLayer(a);
}
