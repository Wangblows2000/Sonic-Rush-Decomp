#include <nitro/types.h>

void DisposeCallback(void *a, u32 b, u32 c);
void SND_InvalidateSeqData(void *a, void *b);

void SeqDisposeCallback(u8 *a, u32 b, u32 c, u32 d) {
    DisposeCallback(a, c, d);
    SND_InvalidateSeqData(a, a + b);
}
