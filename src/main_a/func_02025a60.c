#include <nitro/types.h>

void G3X_Init(void);
void NNS_G3dGlbInit(void);

void func_02025a60(void) {
    G3X_Init();
    NNS_G3dGlbInit();
    *(vu32 *)0x04000600 = (*(vu32 *)0x04000600 & ~0xC0000000) | 0x80000000;
}
