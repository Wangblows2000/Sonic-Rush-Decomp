#include <nitro/types.h>

extern u32 data_0207d3a4;
extern u16 data_0207d3a8[];

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
s32 OsCountZeroBits(u32 x);

void func_020078c8(u16 lockBit, u16 lockID)
{
    u32 bits;
    u32 enabled = OS_DisableInterrupts();
    bits = lockBit & data_0207d3a4 & 0x1ff;

    while (TRUE) {
        s32 lowBit = 31 - OsCountZeroBits(bits);
        u32 bit;
        if (lowBit < 0) {
            break;
        }
        bit = ~(1 << lowBit);
        bits &= bit;
        if (data_0207d3a8[lowBit] == lockID) {
            data_0207d3a8[lowBit] = 0;
            data_0207d3a4 &= bit;
        }
    }
    (void)OS_RestoreInterrupts(enabled);
}
