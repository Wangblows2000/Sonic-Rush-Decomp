#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
    u16 unke;
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 unk18;
    u8 pad1a[0x18];
    u16 unk32;
    u16 unk34;
    u16 unk36;
} UnkStruct_022c1a00;

extern s32 sSysState;
extern u16 data_022c1938;
extern u16 data_022c193c;
extern u16 data_022c1948;
extern u16 data_022c1950;
extern s32 data_022c1964;
extern s32 data_022c196c;
extern s32 data_022c1990;
extern void (*data_022c197c)(const char *fmt, ...);
extern UnkStruct_022c1a00 sParentParam;
extern char data_0207874c[];
extern char data_02078764[];

void OS_Terminate(void);
void WH_ChangeSysState(s32 a);
u16 func_02011098(void);
void WH_Alloc(void);
void WFS_InitParent(s32 a, s32 b, void (*cb)(void), s32 d, u16 e, s32 f, s32 g);
void WFS_SetDebugMode(s32 a);
void WFS_EnableSync(s32 a);
BOOL WH_StateInSetParentParam(void);

BOOL WH_ParentConnect(s32 mode, u16 a, u16 b) {
    s32 x;
    s32 y;

    if (sSysState != 1)
        OS_Terminate();
    if (mode == 6) {
        x = (data_022c1938 + 0x23) & ~0x1f;
        y = (data_022c193c + 0x21) & ~0x1f;
        if (x <= y)
            x = y;
        data_022c1964 = x;
        x = (((data_022c193c + 0xe) * data_022c1948 + 0x29) & ~0x1f) * 2;
        y = ((data_022c1938 + 0x55) & ~0x1f) * 2;
        if (x <= y)
            x = y;
        data_022c1990 = x;
    } else {
        data_022c1990 = (((data_022c1950 + 0xe) * data_022c1948 + 0x29) & ~0x1f) * 2;
        data_022c1964 = (data_022c1950 * (data_022c1948 + 1) + 0x27) & ~0x1f;
    }
    data_022c196c = mode;
    WH_ChangeSysState(3);
    sParentParam.unkc = a;
    sParentParam.unk32 = b;
    sParentParam.unk18 = func_02011098();
    if (mode == 6) {
        sParentParam.unk34 = data_022c1938;
        sParentParam.unk36 = data_022c193c;
    } else {
        sParentParam.unk34 = data_022c1950 * (data_022c1948 + 1) + 4;
        sParentParam.unk36 = data_022c1950;
    }
    sParentParam.unk10 = data_022c1948;
    sParentParam.unk16 = (mode == 6) ? 1 : 0;
    sParentParam.unk12 = 0;
    sParentParam.unke = 1;
    sParentParam.unk14 = (mode == 2) ? 1 : 0;
    switch (mode) {
    case 6:
        if (data_022c197c != NULL)
            data_022c197c(data_0207874c);
        WFS_InitParent(1, 0, WH_Alloc, 0, data_022c1938, 0, 1);
        WFS_SetDebugMode(1);
        WFS_EnableSync(0);
    case 0:
    case 2:
    case 4:
        return WH_StateInSetParentParam();
    default:
        if (data_022c197c != NULL)
            data_022c197c(data_02078764, mode);
        return FALSE;
    }
}
