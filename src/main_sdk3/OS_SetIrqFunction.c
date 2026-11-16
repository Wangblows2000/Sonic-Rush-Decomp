#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    u32 enable;
    void *arg;
} UnkIrqCbInfo;

extern UnkIrqCbInfo data_0207d01c[];

typedef void (*UnkIrqFunc)(void);

extern UnkIrqFunc data_027e0004[];

void OS_SetIrqFunction(u32 intrBit, UnkIrqFunc function)
{
    s32 i;
    UnkIrqCbInfo *info;

    for (i = 0; i < 22; i++) {
        if (intrBit & 1) {
            info = NULL;
            if (8 <= i && i <= 11) {
                info = &data_0207d01c[(u32)(i - 8)];
            } else if (3 <= i && i <= 6) {
                info = &data_0207d01c[(u32)(i - 3 + 4)];
            } else {
                data_027e0004[i] = function;
            }
            if (info) {
                info->func = (void (*)(void *))function;
                info->arg = 0;
                info->enable = TRUE;
            }
        }
        intrBit >>= 1;
    }
}
