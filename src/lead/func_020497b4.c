#include <nitro/types.h>

extern u32 data_022c4488;
extern u32 data_ov000_02369ba0;
extern u8 data_0207a054[];

void func_02021310(void *ctx, void *desc);
u32 func_02021264(void *ctx, u32 idx);
void func_020257f8(u32 value);
void func_020212e0(void *ctx);

void func_020497b4(void) {
    u8 ctx[0x5C];
    data_ov000_02369ba0 = data_022c4488;
    func_02021310(ctx, data_0207a054);
    func_020257f8(func_02021264(ctx, 0));
    func_020212e0(ctx);
}
