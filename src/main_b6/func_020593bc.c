#include <nitro/types.h>

typedef struct { u8 pad[3]; u8 flags; } St;
extern St data_022c4994;
extern s32 data_02087ccc;
extern u8 data_022c4680[];
s32 func_02058ab0(void *a);
BOOL func_02058474(void);

s32 func_020593bc(void) {
    s32 r;
    data_02087ccc = 1;
    if ((data_022c4994.flags & 0x80) != 0) {
        data_02087ccc = 0;
        return 6;
    }
    data_022c4994.flags |= 1;
    r = func_02058ab0(data_022c4680);
    if (func_02058474() == 0) {
        data_02087ccc = 0;
        return 3;
    }
    data_02087ccc = 0;
    return r;
}
