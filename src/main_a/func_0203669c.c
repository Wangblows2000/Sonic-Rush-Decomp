#include <nitro/types.h>

extern u32 data_02250468[];
extern u32 data_02250460;
extern u8 data_0225045c[];
void func_0201bf34(void *p);
void MI_CpuFill8(void *dest, u8 data, u32 size);

void func_0203669c(void) {
    u32 i;
    for (i = 0; i < 0x20; i++) {
        func_0201bf34(&data_02250468[i]);
    }
    func_0201bf34(&data_02250460);
    MI_CpuFill8(data_0225045c, 0, 4);
}
