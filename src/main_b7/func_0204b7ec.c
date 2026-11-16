#include <nitro/types.h>

extern void *data_02250454;
extern u8 data_02250444;
void func_0201bf1c(void *a);
void func_0201bf34(void *a);
void func_0201e9cc(void *a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_0201be98(void *a, s32 v);

void func_0204b7ec(void **a, s32 b, s32 c) {
    if (a == 0)
        a = &data_02250454;
    if (data_02250454 != 0)
        func_0201bf1c(&data_02250454);
    func_0201bf34(a);
    func_0201e9cc(a, -1, -1, -1, b, c);
    func_0201be98(a, data_02250444);
}
