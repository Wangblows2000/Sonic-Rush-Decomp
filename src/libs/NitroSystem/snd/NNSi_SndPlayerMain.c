#include <nitro/types.h>
#include <NitroSystem/sndp.h>

extern u8 data_02083458[];
extern s16 data_020725e0[];

u32 SND_GetPlayerStatus(void);
BOOL SND_IsFinishedCommandTag(u32 id);
void ShutdownPlayer_snd_player(UnkSndPlayer *p);
void NNSi_SndFaderUpdate(UnkSndFader *f);
s32 NNSi_SndFaderGet(UnkSndFader *f);
void SND_SetPlayerVolume(u32 player, s32 vol);
BOOL NNSi_SndFaderIsFinished(UnkSndFader *f);
void ForceStopSeq(UnkSndPlayer *p);
void SND_StartPreparedSeq(u32 player);

void NNSi_SndPlayerMain(void)
{
    u32 status = SND_GetPlayerStatus();
    UnkSndPlayer *p;
    UnkSndPlayer *next;
    s32 vol;
    s32 v2;
    s32 v1;
    s32 v0;

    p = NNS_FndGetNextListObject(data_02083458, NULL);
    if (p == NULL) {
        return;
    }
    do {
        next = NNS_FndGetNextListObject(data_02083458, p);
        if (!p->prepared) {
            if (SND_IsFinishedCommandTag(p->seqId)) {
                p->prepared = TRUE;
            }
        }
        if (p->prepared && !(status & (1 << p->playerNo))) {
            ShutdownPlayer_snd_player(p);
        } else {
            NNSi_SndFaderUpdate(&p->fader);
            v2 = data_020725e0[p->volume2];
            v1 = data_020725e0[p->volume1];
            v0 = data_020725e0[p->info->volume];
            vol = data_020725e0[NNSi_SndFaderGet(&p->fader) >> 8];
            vol += v1 + v2 + v0;
            if (vol < -723) {
                vol = -723;
            } else if (vol > 0) {
                vol = 0;
            }
            if (vol != p->volume) {
                SND_SetPlayerVolume(p->playerNo, vol);
                p->volume = vol;
            }
            if (p->status == 2 && NNSi_SndFaderIsFinished(&p->fader)) {
                ForceStopSeq(p);
            }
            if (p->pauseReq) {
                SND_StartPreparedSeq(p->playerNo);
                p->pauseReq = FALSE;
            }
        }
        p = next;
    } while (p != NULL);
}
