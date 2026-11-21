#include <nitro/types.h>

u8 *SND_GetWaveDataAddress(u32 a, u32 b);
void SND_SetWaveDataAddress(u32 a, u32 b, u32 c);
void SND_InvalidateWaveData(void *a, void *b);

void SingleWaveDisposeCallback(u8 *a, u32 b, u32 c, u32 d) {
    if (a == SND_GetWaveDataAddress(c, d)) {
        SND_SetWaveDataAddress(c, d, 0);
    }
    SND_InvalidateWaveData(a, a + b);
}
