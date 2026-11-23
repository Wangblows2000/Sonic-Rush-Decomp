/*
 * NitroSystem sound player layer (native). Function names and logic follow
 * src/libs/NitroSystem/snd/ (see docs/dreamcast/AUDIO.md mapping table).
 */
#ifndef SND_NNS_H
#define SND_NNS_H

#include "snd_internal.h"

typedef int BOOL_;

/* NNS_Fnd intrusive list (native replacement of NNSFndList / NNSFndLink) */
typedef struct NNSFndLink { void *prev, *next; } NNSFndLink;
typedef struct NNSFndList { void *head, *tail; uint16_t numObjects, offset; } NNSFndList;
void NNS_FndInitList(NNSFndList *list, uint16_t offset);
void NNS_FndAppendListObject(NNSFndList *list, void *obj);
void NNS_FndInsertListObject(NNSFndList *list, void *before, void *obj);
void NNS_FndRemoveListObject(NNSFndList *list, void *obj);
void *NNS_FndGetNextListObject(NNSFndList *list, void *obj);

typedef struct { int32_t origin, target, counter, frame; } NNSSndFader;

struct NNSSndSeqPlayer;
typedef struct NNSSndHandle { struct NNSSndSeqPlayer *player; } NNSSndHandle;

/* data_020838a4[32] (NNSSndPlayer: per player number) */
typedef struct NNSSndPlayer {
    NNSFndList playerList;     /* seq players playing on this player number */
    uint32_t playableSeqCount; /* +0x18 max (SDAT player info seqMax) */
    uint32_t allocChBitFlag;   /* +0x1c */
    uint8_t volume;            /* +0x20 (0x7f) */
} NNSSndPlayer;

/* data_02083464[16] (NNSSndSeqPlayer) */
typedef struct NNSSndSeqPlayer {
    NNSSndHandle *handle;   /* +0x00 */
    NNSSndPlayer *player;   /* +0x04 */
    NNSFndLink prioLink;    /* list link in data_02083458 / data_0208344c */
    NNSFndLink playerLink;  /* list link in NNSSndPlayer.playerList */
    NNSSndFader fader;      /* +0x1c */
    uint8_t status;         /* +0x2c 0 free, 1 playing, 2 fading out (stop requested) */
    uint8_t prepared;       /* +0x2d */
    uint8_t unk2e;
    uint8_t pauseReq;       /* +0x2f start request (SND_StartPreparedSeq on next main) */
    uint32_t seqId;         /* +0x30 command tag of SND_PrepareSeq (native: 1 when issued) */
    uint8_t playerNo;       /* +0x3c driver player 0..15 */
    uint8_t prio;           /* +0x3d */
    int16_t volume;         /* +0x3e last SND_SetPlayerVolume value (0.1 dB) */
    uint8_t volume1;        /* +0x40 initial volume (SEQ info volume) */
    uint8_t volume2;        /* +0x41 NNS_SndPlayerSetVolume */
    int32_t seqArcNo, seqNo;/* NNS_SndPlayerSetSeqArcNo */
} NNSSndSeqPlayer;

void NNSi_SndPlayerInit(void);
void NNS_SndArcPlayerSetup(const SndBank *bank);
void NNSi_SndPlayerMain(void);
BOOL_ NNS_SndArcPlayerStartSeqArcEx(NNSSndHandle *handle, int32_t playerNo, int32_t bankNo, int32_t prio,
                                    int32_t seqArcNo, int32_t index);
BOOL_ NNS_SndArcPlayerStartSeqArc(NNSSndHandle *handle, int32_t seqArcNo, int32_t index);
void NNS_SndPlayerStopSeq(NNSSndHandle *handle, int32_t fadeFrames);
void NNS_SndPlayerStopSeqAll(int32_t fadeFrames);
void NNS_SndPlayerStopSeqBySeqArcIdx(int32_t seqArcNo, int32_t index, int32_t fadeFrames);
int32_t NNS_SndPlayerCountPlayingSeqBySeqArcIdx(int32_t seqArcNo, int32_t index);
void NNS_SndPlayerSetVolume(NNSSndHandle *handle, int volume);
void NNS_SndPlayerSetInitialVolume(NNSSndHandle *handle, int volume);
void NNS_SndPlayerSetChannelPriority(NNSSndHandle *handle, int prio);
void NNS_SndPlayerSetTrackPan(NNSSndHandle *handle, uint32_t trackMask, int pan);
void NNS_SndPlayerSetTempoRatio(NNSSndHandle *handle, int ratio);   /* 256 = normal */
void NNS_SndHandleInit(NNSSndHandle *handle);
void NNS_SndHandleReleaseSeq(NNSSndHandle *handle);
int NNS_SndPlayerCountPlaying(int playerNo); /* native: playerList.numObjects */
void NNSi_SndPlayerPause(NNSSndSeqPlayer *p, int flag);
void NNS_SndPlayerPause(NNSSndHandle *handle, int flag);
void NNS_SndPlayerPauseByPlayerNo(int playerNo, int flag);

#endif
