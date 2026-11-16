#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 flags;
} UnkState_0205943c;

extern s32 data_02087ccc;
extern UnkState_0205943c data_022c4994;

void func_02058474(void);
void func_020593bc(void);
s32 func_02058e34(void);
s32 func_0205899c(void);

s32 func_0205943c(void) {
    data_02087ccc = 1;
    func_02058474();
    if (data_022c4994.flags & 0x80) {
        data_02087ccc = 0;
        return 6;
    }
    if ((data_022c4994.flags & 1) == 0) {
        func_020593bc();
        data_02087ccc = 0;
        return 0;
    }
    if (func_02058e34() == 0) {
        data_02087ccc = 0;
        return 0;
    }
    if (func_0205899c()) {
        data_02087ccc = 0;
        return 2;
    } else {
        data_02087ccc = 0;
        return 0;
    }
}
