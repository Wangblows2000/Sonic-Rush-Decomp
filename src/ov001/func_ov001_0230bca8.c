#include <nitro/types.h>

extern u8 *data_ov001_0231f5c4;
void SpecialStage_RelocateSsdHeader(void *p);

void func_ov001_0230bca8(void) {
    SpecialStage_RelocateSsdHeader(*(void **)(data_ov001_0231f5c4 + 0x2c));
}
