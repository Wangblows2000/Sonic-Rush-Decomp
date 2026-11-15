#include <nitro/types.h>

extern u32 data_02250450;
extern u8 data_02250448;
void func_0201bf1c(void *p);
void func_0201bf34(void *p);
void func_0201ea88(void *p, u32 a, u32 b);
void func_0201be98(void *p, u8 v);

void func_020366f4(u32 *p, u32 a, u32 b) {
    if (p == NULL) p = &data_02250450;
    if (data_02250450 != 0) func_0201bf1c(&data_02250450);
    func_0201bf34(p);
    func_0201ea88(p, a, b);
    func_0201be98(p, data_02250448);
}
