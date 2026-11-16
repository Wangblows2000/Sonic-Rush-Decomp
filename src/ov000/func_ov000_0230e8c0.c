#include <nitro/types.h>

extern u8 data_ov000_0236a024;

void ObjData_Release(void *p);
void func_ov000_022fb9c8(void *p);

void func_ov000_0230e8c0(void *p) {
    ObjData_Release(&data_ov000_0236a024);
    func_ov000_022fb9c8(p);
}
