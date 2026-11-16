#include <nitro/types.h>

typedef struct {
    s32 mode;
} UnkWork_022d2db8;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d2db8 *work;
} UnkTask_022d2db8;

extern UnkTask_022d2db8 *data_022b4574;
extern s32 data_ov001_0231f050;

void func_02036fdc(s32 a);
void func_02037d40(void);
void func_02059bdc(s32 a);
void func_02059bcc(s32 a);
void func_02059b2c(void);
BOOL func_0205a4ec(s32 a);
void func_02054e30(void);

#pragma thumb on
void func_ov001_022d2db8(void) {
    s32 mode;

    mode = data_022b4574->work->mode;
    data_ov001_0231f050 = 0;
    func_02036fdc(0x1173);
    func_02037d40();
    switch (mode) {
    case 0:
        func_02059bdc(0x1a);
        break;
    case 1:
        func_02059bcc(0x1c);
        break;
    case 2:
        func_02059b2c();
        break;
    }
    if (func_0205a4ec(0)) {
        func_02054e30();
    }
}
#pragma thumb reset
