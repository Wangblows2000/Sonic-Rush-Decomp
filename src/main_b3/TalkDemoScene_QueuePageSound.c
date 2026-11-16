#include <nitro/types.h>

typedef struct { u8 data[0xc]; } UnkEnt_02076e58;
typedef struct { u8 pad0[4]; u32 mode; } UnkInfo_020611a8;
typedef struct {
    u8 pad0[4];
    u32 flags;
    u8 pad8[0x64];
    UnkInfo_020611a8 *info;  /* 0x6c */
    u8 pad70[0x60];
    void *unkD0;             /* 0xd0 */
} UnkWork_020611a8;
typedef struct { u8 pad0[0x30]; void *snd0; void *snd1; } UnkPlayer_020611a8;
typedef struct { u8 pad0[0xc]; s16 unkC; } UnkStruct_02075a34;

extern s16 gTalkDemoId;
extern u8 data_02076ee8[];
extern UnkEnt_02076e58 data_02076e58[];
extern s32 data_02076d60[];
extern UnkStruct_02075a34 data_02075a34;

UnkPlayer_020611a8 *PlayerDemo_GetWork(void);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void NNS_SndHandleReleaseSeq(void *p);
void Sound_PlaySeq(void *a, s32 b);

#pragma thumb on
void TalkDemoScene_QueuePageSound(UnkWork_020611a8 *w) {
    void **dst = &w->unkD0;
    u32 mode = w->info->mode;

    if (gTalkDemoId == 0x37) {
        *dst = data_02076ee8;
        w->flags |= 0x100;
        return;
    }
    if (mode >= 6 && mode <= 0xf) {
        if (mode == 0xf) {
            UnkPlayer_020611a8 *p = PlayerDemo_GetWork();
            if (p->snd0 != NULL) {
                NNS_SndPlayerStopSeq(&p->snd0, 0);
                NNS_SndHandleReleaseSeq(&p->snd0);
            }
            if (p->snd1 != NULL)
                NNS_SndPlayerStopSeq(&p->snd1, 0);
            Sound_PlaySeq(&p->snd0, data_02075a34.unkC);
        } else {
            *dst = &data_02076e58[data_02076d60[mode]];
            w->flags |= 0x100;
        }
    }
}
#pragma thumb reset
