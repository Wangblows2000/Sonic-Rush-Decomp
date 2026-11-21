#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 fader[0x10];
    u8 status;
} UnkSndPlayer_0201b98c;

void ForceStopSeq(UnkSndPlayer_0201b98c *p);
void NNSi_SndFaderSet(void *fader, s32 target, s32 frames);
void SetPlayerPriority(UnkSndPlayer_0201b98c *p, u8 prio);

void NNSi_SndPlayerStopSeq(UnkSndPlayer_0201b98c *p, s32 frames)
{
    if (p == NULL) {
        return;
    }
    if (p->status == 0) {
        return;
    }
    if (frames == 0) {
        ForceStopSeq(p);
        return;
    }
    NNSi_SndFaderSet(p->fader, 0, frames);
    SetPlayerPriority(p, 0);
    p->status = 2;
}
