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

extern s32 data_022c1980;
extern u16 data_022c1938;
extern u16 data_022c193c;
extern u16 data_022c1948;
extern u16 data_022c1950;
extern s32 data_022c1964;
extern s32 data_022c196c;
extern s32 data_022c1990;
extern void (*data_022c197c)(const char *fmt, ...);
extern UnkStruct_022c1a00 data_022c1a00;
extern char data_0207874c[];
extern char data_02078764[];

void OS_Terminate(void);
void func_020405e0(s32 a);
u16 func_02011098(void);
void func_020404bc(void);
void func_0203c5cc(s32 a, s32 b, void (*cb)(void), s32 d, u16 e, s32 f, s32 g);
void func_0203c244(s32 a);
void func_0203c254(s32 a);
BOOL func_02040408(void);

BOOL func_0203e10c(s32 mode, u16 a, u16 b) {
    s32 x;
    s32 y;

    if (data_022c1980 != 1)
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
    func_020405e0(3);
    data_022c1a00.unkc = a;
    data_022c1a00.unk32 = b;
    data_022c1a00.unk18 = func_02011098();
    if (mode == 6) {
        data_022c1a00.unk34 = data_022c1938;
        data_022c1a00.unk36 = data_022c193c;
    } else {
        data_022c1a00.unk34 = data_022c1950 * (data_022c1948 + 1) + 4;
        data_022c1a00.unk36 = data_022c1950;
    }
    data_022c1a00.unk10 = data_022c1948;
    data_022c1a00.unk16 = (mode == 6) ? 1 : 0;
    data_022c1a00.unk12 = 0;
    data_022c1a00.unke = 1;
    data_022c1a00.unk14 = (mode == 2) ? 1 : 0;
    switch (mode) {
    case 6:
        if (data_022c197c != NULL)
            data_022c197c(data_0207874c);
        func_0203c5cc(1, 0, func_020404bc, 0, data_022c1938, 0, 1);
        func_0203c244(1);
        func_0203c254(0);
    case 0:
    case 2:
    case 4:
        return func_02040408();
    default:
        if (data_022c197c != NULL)
            data_022c197c(data_02078764, mode);
        return FALSE;
    }
}
