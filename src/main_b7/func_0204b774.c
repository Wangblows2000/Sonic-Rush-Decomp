#include <nitro/types.h>

extern void *gSESoundHandle;
extern u8 gSEVolume;
void func_0201bf1c(void *a);
void func_0201bf34(void *a);
void func_0201e9cc(void *a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_0201be98(void *a, s32 v);

void func_0204b774(void **a, s32 b, s32 c, s32 d, s32 e) {
    if (a == 0)
        a = &gSESoundHandle;
    if (gSESoundHandle != 0)
        func_0201bf1c(&gSESoundHandle);
    func_0201bf34(a);
    func_0201e9cc(a, -1, e, d, b, c);
    func_0201be98(a, gSEVolume);
}
