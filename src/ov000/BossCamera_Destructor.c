#include <nitro/types.h>

extern u8 data_ov000_0236a524;

void Model_Release(void *p);

void BossCamera_Destructor(void) {
    Model_Release(&data_ov000_0236a524);
}
