#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 flags;
} UnkData_02059340;

extern UnkData_02059340 data_022c4994;
extern s32 data_02087ccc;
extern u8 data_022c4680[];

void func_020586d4(void *p);
BOOL func_02058d24(void);

s32 func_02059340(void) {
    data_02087ccc = 1;
    if (data_022c4994.flags & 0x80) {
        data_02087ccc = 0;
        return 6;
    }
    func_020586d4(data_022c4680);
    if (func_02058d24() == 0) {
        data_02087ccc = 0;
        return 2;
    }
    data_02087ccc = 0;
    return 0;
}
