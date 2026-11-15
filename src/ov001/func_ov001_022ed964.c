#include <nitro/types.h>

void func_02037d40(void *p);
void func_ov001_022eca98(void *p);
void func_ov001_022ed960(void *p);
void func_ov001_022ed924(void *p);
void func_ov001_022ed8b0(void *p);
void func_ov001_022ec41c(void);

#pragma thumb on
void func_ov001_022ed964(u8 *p) {
    void *q = *(void **)(p + 0x10);
    func_02037d40(p);
    func_ov001_022eca98(q);
    func_ov001_022ed960(q);
    func_ov001_022ed924(q);
    func_ov001_022ed8b0(q);
    func_ov001_022ec41c();
}
#pragma thumb reset
