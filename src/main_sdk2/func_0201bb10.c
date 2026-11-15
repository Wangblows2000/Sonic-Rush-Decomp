#include <nitro/types.h>
#include <main_sdk2/sndp.h>

extern u8 data_02083458[];
extern s16 data_020725e0[];

u32 func_02009ab8(void);
BOOL func_02009158(u32 id);
void func_0201b698(UnkSndPlayer *p);
void NNSi_SndFaderUpdate(UnkSndFader *f);
s32 func_02020564(UnkSndFader *f);
void SND_SetPlayerVolume(u32 player, s32 vol);
BOOL NNSi_SndFaderIsFinished(UnkSndFader *f);
void func_0201b7a4(UnkSndPlayer *p);
void func_02008e48(u32 player);

void func_0201bb10(void)
{
    u32 status = func_02009ab8();
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
            if (func_02009158(p->seqId)) {
                p->prepared = TRUE;
            }
        }
        if (p->prepared && !(status & (1 << p->playerNo))) {
            func_0201b698(p);
        } else {
            NNSi_SndFaderUpdate(&p->fader);
            v2 = data_020725e0[p->volume2];
            v1 = data_020725e0[p->volume1];
            v0 = data_020725e0[p->info->volume];
            vol = data_020725e0[func_02020564(&p->fader) >> 8];
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
                func_0201b7a4(p);
            }
            if (p->pauseReq) {
                func_02008e48(p->playerNo);
                p->pauseReq = FALSE;
            }
        }
        p = next;
    } while (p != NULL);
}
