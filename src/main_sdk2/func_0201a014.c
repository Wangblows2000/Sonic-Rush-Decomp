#include <nitro/types.h>
#include <main_sdk2/wh.h>

extern u16 data_020779c0;
extern u16 data_020779c4;
extern u16 data_020779c8;
extern u16 data_020779cc;

void func_020197bc(UnkCallback_wh *arg);
void func_020182e8(u16 id, s32 result);
s32 func_0201154c(void (*cb)(UnkCallback_wh *));
s32 func_02012ec0(void (*cb)(UnkCallback_wh *), u16 a, u16 b, u16 c, u16 d);

void func_0201a014(void)
{
    func_020182e8(0x80, func_0201154c(func_020197bc));
    func_020182e8(0x1d, func_02012ec0(func_020197bc, data_020779cc, data_020779c4, data_020779c0, data_020779c8));
}
