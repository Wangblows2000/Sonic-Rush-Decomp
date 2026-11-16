#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 b2;
} UnkState_02058ab0;

extern UnkState_02058ab0 data_022c4994;

BOOL func_02058b50(u8 slot, void *buf);
BOOL func_02058898(void *buf);

s32 func_02058ab0(void *buf) {
    BOOL f = FALSE;
    u32 s = data_022c4994.b2 << 1;

    if (func_02058b50(s, buf)) {
        if (func_02058898(buf))
            return 0;
    } else {
        f = TRUE;
    }
    if (func_02058b50(s + 1, buf)) {
        if (func_02058898(buf))
            return 0;
    } else if (f) {
        return 1;
    }
    return 3;
}
