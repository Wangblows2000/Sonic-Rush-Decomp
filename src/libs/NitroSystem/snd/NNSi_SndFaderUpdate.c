#include <nitro/types.h>

typedef struct { s32 origin; s32 target; s32 counter; s32 frame; } NNSSndFader;

void NNSi_SndFaderUpdate(NNSSndFader *fader) {
    if (fader->counter < fader->frame) {
        fader->counter++;
    }
}
