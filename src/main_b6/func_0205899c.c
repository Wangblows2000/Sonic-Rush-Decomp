#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 b2;
} UnkState_0205899c;

extern UnkState_0205899c data_022c4994;
extern u8 data_022c4680[];

BOOL func_020588f8(void);
BOOL func_02058a44(u8 slot, void *buf);

s32 func_0205899c(void) {
    u8 start = data_022c4994.b2;

    if (!func_020588f8())
        return 2;
    while (start != data_022c4994.b2) {
        if (func_02058a44(data_022c4994.b2 << 1, data_022c4680) && func_02058a44((data_022c4994.b2 << 1) + 1, data_022c4680))
            return 0;
        if (!func_020588f8())
            return 2;
    }
    return 2;
}
