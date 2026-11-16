#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void (*func)(void);
} UnkTask_022c8674;

extern UnkTask_022c8674 *data_022b4574;
extern s32 data_ov001_0231f044;

s32 func_0204b6cc(void);
void func_ov001_022c8640(void);
void func_ov001_022c87f8(void);

#pragma thumb on
void func_ov001_022c8674(void) {
    switch (func_0204b6cc()) {
    case 0:
        break;
    case 1:
        data_022b4574->func = func_ov001_022c8640;
        break;
    case 2:
        func_ov001_022c87f8();
        data_ov001_0231f044 = 1;
        break;
    }
}
#pragma thumb reset
