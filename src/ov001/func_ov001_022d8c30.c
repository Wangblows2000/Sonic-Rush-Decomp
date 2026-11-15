#include <nitro/types.h>

void func_02037d40(void *p);
void func_ov001_022d8c78(void *p);

#pragma thumb on
void func_ov001_022d8c30(u8 *p) {
    void *q = *(void **)(p + 0x10);
    func_02037d40(p);
    func_ov001_022d8c78(q);
}
#pragma thumb reset
