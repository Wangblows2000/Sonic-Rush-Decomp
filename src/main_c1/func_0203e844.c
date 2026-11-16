#include <nitro/types.h>

extern u32 data_022c1988;
extern u16 data_022c1960;
extern u16 data_022c1958;

void func_020078a4(u16 *mac);
void func_020405e0(s32 a);
void func_020405bc(s32 v);
s32 func_0203e790(u16 ch);

BOOL func_0203e844(void)
{
    u16 mac[4];
    u32 seed;
    s32 r;

    func_020078a4(mac);
    seed = *(u32 *)0x027ffc3c;
    seed = mac[2] + (mac[1] + (mac[0] + seed));
    *(volatile u32 *)&data_022c1988 = seed;
    data_022c1988 = seed * 0x10dcd + 0x3039;
    data_022c1960 = 0;
    data_022c1958 = 0x65;
    func_020405e0(3);
    r = func_0203e790(1);
    if (r == 0x18) {
        func_020405bc(0x18);
        func_020405e0(9);
        return FALSE;
    }
    if (r == 2)
        return TRUE;
    func_020405bc(r);
    func_020405e0(9);
    return FALSE;
}
