#include <nitro/types.h>

s32 func_0200a0d8(u32 fifo, u32 data, BOOL err);
void WaitByLoop(s32 count);

void func_0201a9dc(u32 data)
{
    while (func_0200a0d8(13, data, FALSE) != 0) {
        WaitByLoop(1);
    }
}
