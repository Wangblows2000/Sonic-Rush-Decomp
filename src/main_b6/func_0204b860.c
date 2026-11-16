#include <nitro/types.h>

extern void *data_02250464;
extern u8 data_0225044c;
void func_0201c070(void *a, s32 b);
void func_0201bf1c(void *a);
void func_0201bf34(void *a);
void func_0201eb20(void *a, s32 b, s32 c, s32 d, s32 e);
void func_0201be98(void *a, s32 v);

void func_0204b860(void **a, s32 b, s32 c, s32 d) {
    if (a == 0)
        a = &data_02250464;
    if (data_02250464 != 0) {
        func_0201c070(&data_02250464, 0);
        func_0201bf1c(&data_02250464);
    }
    func_0201bf34(a);
    func_0201eb20(a, -1, d, c, b);
    func_0201be98(a, data_0225044c);
}
