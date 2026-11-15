#include <nitro/types.h>

typedef struct { s32 origin; s32 target; s32 counter; s32 frame; } NNSSndFader;

s32 func_02020564(NNSSndFader *f) {
    if (f->counter >= f->frame) return f->target;
    return f->origin + (f->target - f->origin) * f->counter / f->frame;
}
