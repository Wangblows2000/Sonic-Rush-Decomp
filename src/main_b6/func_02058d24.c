#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 b2;
} UnkState_02058d24;

extern UnkState_02058d24 data_022c4994;

BOOL func_02058bbc(u8 slot);

BOOL func_02058d24(void) {
    BOOL err = FALSE;
    u8 i;

    for (i = 0; i < 5; i++) {
        err |= func_02058bbc(i * 2) == 0;
        err |= func_02058bbc(i * 2 + 1) == 0;
    }
    data_022c4994.b2 = 0;
    return err == FALSE;
}
