#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    u32 enable;
    void *arg;
} UnkIrqCbInfo;

extern UnkIrqCbInfo data_0207d01c[];

typedef void (*UnkIrqFunc)(void);

extern UnkIrqFunc OS_IRQTable[];

UnkIrqFunc OS_GetIrqFunction(u32 intrBit)
{
    s32 i;
    UnkIrqFunc *funcPtr = OS_IRQTable;

    for (i = 0; i < 22; i++) {
        if (intrBit & 1) {
            if (8 <= i && i <= 11) {
                return (UnkIrqFunc)data_0207d01c[(u32)(i - 8)].func;
            } else if (3 <= i && i <= 6) {
                return (UnkIrqFunc)data_0207d01c[(u32)(i - 3 + 4)].func;
            } else {
                return *funcPtr;
            }
        }
        intrBit >>= 1;
        funcPtr++;
    }
    return 0;
}
