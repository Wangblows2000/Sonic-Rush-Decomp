#include <nitro/types.h>

extern void *data_ov001_0231f060;
void func_ov001_022e9308(void *p);
void func_ov001_022e9304(void *p);
void func_ov001_022e92b4(void);

#pragma thumb on
void func_ov001_022e8d5c(u8 *p) {
    void *q = *(void **)(p + 0x10);
    func_ov001_022e9308(q);
    func_ov001_022e9304(q);
    func_ov001_022e92b4();
    if (q == data_ov001_0231f060) {
        data_ov001_0231f060 = NULL;
    }
}
#pragma thumb reset
