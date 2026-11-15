#include <nitro/types.h>

extern void *data_ov001_0231f05c;
void func_ov001_022e14b8(void *p);
void func_ov001_022e1444(void *p);
void func_ov001_022e1400(void);

#pragma thumb on
void func_ov001_022e0ee0(void) {
    func_ov001_022e14b8(data_ov001_0231f05c);
    func_ov001_022e1444(data_ov001_0231f05c);
    func_ov001_022e1400();
    data_ov001_0231f05c = NULL;
}
#pragma thumb reset
