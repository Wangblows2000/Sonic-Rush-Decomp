#include <nitro/types.h>

typedef struct { s32 origin; s32 target; s32 counter; s32 frame; } NNSSndFader;
s32 func_02020564(NNSSndFader *f);

void NNSi_SndFaderSet(NNSSndFader *f, s32 target, s32 frame) {
    f->origin = func_02020564(f);
    f->target = target;
    f->frame = frame;
    f->counter = 0;
}
