#include <nitro/types.h>

void WM_MeasureChannel(void *a, s32 b, s32 c, void *d, s32 e);
void MeasureChannel(void *a, void *d) { WM_MeasureChannel(a, 3, 0x11, d, 0x1e); }
