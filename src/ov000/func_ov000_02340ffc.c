#include <nitro/types.h>

extern u32 gSystemFlags;

void func_ov000_02340ffc(void) {
    gSystemFlags &= ~0xc00;
}
