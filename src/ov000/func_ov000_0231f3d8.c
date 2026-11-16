#include <nitro/types.h>

extern u8 data_ov000_0236a524;

void Model_Release(void *p);

void func_ov000_0231f3d8(void) {
    Model_Release(&data_ov000_0236a524);
}
