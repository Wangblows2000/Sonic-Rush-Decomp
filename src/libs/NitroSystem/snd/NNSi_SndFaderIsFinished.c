#include <nitro/types.h>

typedef struct { s32 origin; s32 target; s32 counter; s32 frame; } NNSSndFader;

BOOL NNSi_SndFaderIsFinished(NNSSndFader *fader) {
    return fader->counter >= fader->frame;
}
