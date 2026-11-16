#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 flags;
} UnkState_020594f8;

extern s32 data_02087ccc;
extern UnkState_020594f8 data_022c4994;

s32 func_02058df0(void);
s32 func_02058dac(void);
BOOL func_020587d4(void);
BOOL func_0200f29c(void);
s32 func_02059340(void);
BOOL func_02058774(void);
s32 func_02059054(void);

s32 func_020594f8(void) {
    data_02087ccc = 1;
    if (func_02058df0() == 0) {
        data_02087ccc = 0;
        return 5;
    }
    if (func_02058dac() == 0)
        return 1;
    data_022c4994.flags &= ~0x80;
    if (func_020587d4() == 0) {
        if (func_0200f29c()) {
            data_02087ccc = 0;
            return 1;
        }
        if (func_02059340()) {
            data_02087ccc = 0;
            return 1;
        }
        func_02058774();
        if (func_0200f29c()) {
            data_02087ccc = 0;
            return 1;
        } else {
            data_02087ccc = 0;
            return 4;
        }
    } else {
        if (func_02059054() == 0) {
            data_02087ccc = 0;
            return 3;
        } else {
            data_02087ccc = 0;
            return 0;
        }
    }
}
