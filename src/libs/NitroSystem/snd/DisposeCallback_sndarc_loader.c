#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
u32 NNS_SndArcSetCurrent(u32 v);
u32 NNS_SndArcGetFileAddress(u32 idx);
void NNS_SndArcSetFileAddress(u32 idx, u32 v);

void DisposeCallback_sndarc_loader(u32 a, u32 b, u32 idx) {
    u32 old, e;
    if (b == 0) return;
    e = OS_DisableInterrupts();
    old = NNS_SndArcSetCurrent(b);
    if (a == NNS_SndArcGetFileAddress(idx)) {
        NNS_SndArcSetFileAddress(idx, 0);
    }
    NNS_SndArcSetCurrent(old);
    OS_RestoreInterrupts(e);
}
