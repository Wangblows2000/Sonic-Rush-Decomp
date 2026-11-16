#include <nitro/types.h>

void DisposeCallback_sndarc_loader(void *a, u32 b, u32 c);
void SND_DestroyWaveArc(void *a);

void WaveArcTableDisposeCallback(void *a, u32 b, u32 c, u32 d) {
    DisposeCallback_sndarc_loader(a, c, d);
    SND_DestroyWaveArc(a);
}
