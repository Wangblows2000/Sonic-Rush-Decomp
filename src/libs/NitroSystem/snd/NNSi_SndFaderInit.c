#include <nitro/types.h>

typedef struct { s32 origin; s32 target; s32 counter; s32 frame; } NNSSndFader;

void NNSi_SndFaderInit(NNSSndFader *fader) {
    fader->origin = fader->target = 0;
    fader->counter = fader->frame = 0;
}
