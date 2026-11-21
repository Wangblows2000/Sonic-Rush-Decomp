#include <nitro/types.h>

void DisposeCallback_sndarc_loader(void *a, u32 b, u32 c);
void SND_InvalidateBankData(void *a, void *b);
void SND_DestroyBank(void *a);

void BankDisposeCallback(u8 *a, u32 b, u32 c, u32 d) {
    DisposeCallback_sndarc_loader(a, c, d);
    SND_InvalidateBankData(a, a + b);
    SND_DestroyBank(a);
}
