#include <nitro/types.h>

u32 NNS_SndArcGetFileAddress(u32 idx);
u32 NNS_SndArcGetCurrent(void);
u32 NNSi_SndArcLoadFile(u32 idx, void *func, u32 c, u32 d, u32 e);
void NNS_SndArcSetFileAddress(u32 idx, u32 v);
void BankDisposeCallback(void);

u32 LoadBank(u32 idx, u32 b, BOOL c) {
    u32 r = NNS_SndArcGetFileAddress(idx);
    if (r == 0) {
        r = NNSi_SndArcLoadFile(idx, BankDisposeCallback, c ? NNS_SndArcGetCurrent() : 0, idx, b);
        if (c && r != 0) {
            NNS_SndArcSetFileAddress(idx, r);
        }
    }
    return r;
}
