#include <nitro/types.h>

typedef struct {
    s32 state;
    s32 sel;
} UnkWork_022ce29c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022ce29c *work;
} UnkTask_022ce29c;

extern UnkTask_022ce29c *data_022b4574;

void func_02036fdc(u16 id);
void func_ov001_022c88c8(void);
void func_ov001_022c88b8(void);
void func_ov001_022c88a8(void);
void func_ov001_022c8898(void);
void func_ov001_022c8880(void);

#pragma thumb on
void func_ov001_022ce29c(void) {
    UnkWork_022ce29c *w = data_022b4574->work;
    s32 sel;

    if (w->state == 10) {
        sel = w->sel;
        func_02036fdc(0x1000);
        switch (sel) {
        case 0:
            func_ov001_022c88c8();
            break;
        case 1:
            func_ov001_022c88b8();
            break;
        case 2:
            func_ov001_022c88a8();
            break;
        case 3:
            func_ov001_022c8898();
            break;
        case 4:
            func_ov001_022c8880();
            break;
        }
    }
}
#pragma thumb reset
