#include <nitro/types.h>

extern u32 gSESoundHandle;
extern u8 gSEVolume;
void func_0201bf1c(void *p);
void func_0201bf34(void *p);
void func_0201ea88(void *p, u32 a, u32 b);
void func_0201be98(void *p, u8 v);

void Sound_PlaySeqArc(u32 *p, u32 a, u32 b) {
    if (p == NULL) p = &gSESoundHandle;
    if (gSESoundHandle != 0) func_0201bf1c(&gSESoundHandle);
    func_0201bf34(p);
    func_0201ea88(p, a, b);
    func_0201be98(p, gSEVolume);
}
