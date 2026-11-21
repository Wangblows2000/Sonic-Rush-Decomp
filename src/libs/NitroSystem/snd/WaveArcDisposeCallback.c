#include <nitro/types.h>

void DisposeCallback_sndarc_loader(void *a, u32 b, u32 c);
void SND_InvalidateWaveData(void *a, void *b);
void SND_DestroyWaveArc(void *a);

void WaveArcDisposeCallback(u8 *a, u32 b, u32 c, u32 d) {
    DisposeCallback_sndarc_loader(a, c, d);
    SND_InvalidateWaveData(a, a + b);
    SND_DestroyWaveArc(a);
}
