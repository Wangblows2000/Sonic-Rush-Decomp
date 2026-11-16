#include <nitro/types.h>

typedef struct {
    s32 mode;
} UnkWork_022c8738;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022c8738 *work;
} UnkTask_022c8738;

extern UnkTask_022c8738 *data_022b4574;

void func_0204b718(s32 a);
void func_ov001_022c86f4(void);

#pragma thumb on
void func_ov001_022c8738(void) {
    switch (data_022b4574->work->mode) {
    case 0:
        func_0204b718(1);
        break;
    case 1:
        func_0204b718(2);
        break;
    }
    data_022b4574->func = func_ov001_022c86f4;
}
#pragma thumb reset
