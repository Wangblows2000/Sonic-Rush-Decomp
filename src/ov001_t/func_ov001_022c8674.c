#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void (*func)(void);
} UnkTask_022c8674;

extern UnkTask_022c8674 *gTaskCurrent;
extern s32 data_ov001_0231f044;

s32 StageLoader_LoadZoneFiles(void);
void func_ov001_022c8640(void);
void func_ov001_022c87f8(void);

#pragma thumb on
void func_ov001_022c8674(void) {
    switch (StageLoader_LoadZoneFiles()) {
    case 0:
        break;
    case 1:
        gTaskCurrent->func = func_ov001_022c8640;
        break;
    case 2:
        func_ov001_022c87f8();
        data_ov001_0231f044 = 1;
        break;
    }
}
#pragma thumb reset
