#include <nitro/types.h>

extern u16 data_ov000_0236a3c0;

void func_ov000_023265d8(void *p);

void func_ov000_0231e304(void *p) {
    data_ov000_0236a3c0--;
    func_ov000_023265d8(p);
}
