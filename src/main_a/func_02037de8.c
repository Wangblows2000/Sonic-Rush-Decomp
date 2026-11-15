#include <nitro/types.h>

void func_02009fa8(void);
void func_0200d908(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_0200d6f0(void *cb);
BOOL func_0200d85c(void *p);
void func_0200d714(void *p);
void func_02037c5c(void);
extern u8 data_022b65b4[];
extern u8 data_022b658c[];

void func_02037de8(void) {
    u8 buf[8];
    vu16 z0, z1;
    func_02009fa8();
    func_0200d908();
    z0 = 0;
    MIi_CpuClear16(z0, data_022b65b4, 0x28);
    z1 = 0;
    MIi_CpuClear16(z1, data_022b658c, 0x28);
    func_0200d6f0(func_02037c5c);
    if (!func_0200d85c(buf)) return;
    func_0200d714(buf);
}
