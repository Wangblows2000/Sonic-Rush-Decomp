#include <nitro/types.h>

void func_020078a4(u8 *mac);

u32 func_02018e5c(void)
{
    u8 mac[6];
    s32 i;
    u32 sum;

    func_020078a4(mac);
    for (i = 0, sum = 0; i < 6; i++) {
        sum += mac[i];
    }
    sum = (sum + *(vu32 *)0x027ffc3c) * 7;
    return sum % 20;
}
