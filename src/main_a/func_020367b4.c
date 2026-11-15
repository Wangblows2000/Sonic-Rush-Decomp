#include <nitro/types.h>

extern u32 data_02250464;
extern u8 data_0225044c;
void func_0201c070(void *p, u32 a);
void func_0201bf1c(void *p);
void func_0201bf34(void *p);
void func_0201eb98(void *p, u32 a);
void func_0201be98(void *p, u8 v);

void func_020367b4(u32 *p, u32 a) {
    if (p == NULL) p = &data_02250464;
    if (data_02250464 != 0) {
        func_0201c070(&data_02250464, 0);
        func_0201bf1c(&data_02250464);
    }
    func_0201bf34(p);
    func_0201eb98(p, a);
    func_0201be98(p, data_0225044c);
}
