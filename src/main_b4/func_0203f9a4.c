#include <nitro/types.h>

typedef struct {
    u8 pad0[0x36];
    u16 unk36;
} UnkStruct_022c1ae0;

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u8 pad6[2];
    u16 mac[3];
} UnkStruct_022c19e0;

extern u16 data_022c1938;
extern u16 data_022c193c;
extern u16 data_022c1948;
extern u16 data_022c1950;
extern s32 data_022c1964;
extern s32 data_022c196c;
extern s32 data_022c1990;
extern void *data_022c1968;
extern u16 data_022c194c;
extern u16 data_022c1940;
extern void (*data_022c197c)(const char *fmt, ...);
extern UnkStruct_022c1ae0 data_022c1ae0;
extern UnkStruct_022c19e0 data_022c19e0;
extern char data_020789c8[];

void func_020405e0(s32 a);
void func_020404bc(void);
void func_0203c4e8(s32 a, s32 b, void (*cb)(void), s32 d);
void func_0203c244(s32 a);
BOOL func_0203f7f8(void);

BOOL func_0203f9a4(void *cb, s32 mode, u16 *mac, u16 d) {
    s32 x;
    s32 y;

    if (mode == 7) {
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
        data_022c1990 = (((u32)(data_022c1950 * (data_022c1948 + 1) + 4) + 0x55) & ~0x1f) * 2;
        data_022c1964 = (data_022c1950 + 0x21) & ~0x1f;
    }
    func_020405e0(2);
    data_022c1ae0.unk36 = 1;
    data_022c19e0.mac[2] = mac[2];
    data_022c19e0.mac[1] = mac[1];
    data_022c19e0.mac[0] = mac[0];
    data_022c196c = mode;
    data_022c1968 = cb;
    data_022c194c = d;
    data_022c19e0.unk4 = 0;
    data_022c1940 = 1;
    if (mode == 7) {
        if (data_022c197c != NULL)
            data_022c197c(data_020789c8);
        func_0203c4e8(1, 0, func_020404bc, 0);
        func_0203c244(1);
    }
    if (func_0203f7f8())
        return TRUE;
    func_020405e0(9);
    return FALSE;
}
