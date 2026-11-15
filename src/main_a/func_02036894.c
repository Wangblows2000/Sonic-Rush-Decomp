#include <nitro/types.h>

void func_0201da54(u32 p);
void func_0201db14(u32 p);
u32 func_0201db30(void *p, u32 size);
void func_0201bf34(void *p);
extern u8 data_02250574[];
extern u32 data_02250458;
extern u32 data_02250464;
extern u32 data_02250454;
extern u32 data_02250450;
extern u8 data_0225044c;
extern u8 data_02250444;
extern u8 data_02250448;

void func_02036894(void) {
    func_0201da54(data_02250458);
    func_0201db14(data_02250458);
    data_02250458 = func_0201db30(data_02250574, 0x64000);
    func_0201bf34(&data_02250464);
    func_0201bf34(&data_02250454);
    func_0201bf34(&data_02250450);
    data_0225044c = 0x7F;
    data_02250444 = 0x7F;
    data_02250448 = 0x7F;
}
