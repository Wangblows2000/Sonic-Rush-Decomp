#include <nitro/types.h>

extern u16 data_0207d37c;
typedef struct { u32 a; u32 b; } UnkStruct_0207d380;
extern UnkStruct_0207d380 data_0207d380;
void func_02006edc(u32 n);
u32 func_02005348(u32 m);
void func_0200701c(void) {
    if (data_0207d37c != 0) return;
    data_0207d37c = 1;
    func_02006edc(1);
    data_0207d380.a = 0;
    data_0207d380.b = 0;
    func_02005348(0x10);
}
