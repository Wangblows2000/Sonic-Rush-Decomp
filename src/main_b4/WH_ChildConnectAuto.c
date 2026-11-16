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

void WH_ChangeSysState(s32 a);
void WH_Alloc(void);
void WFS_InitChild(s32 a, s32 b, void (*cb)(void), s32 d);
void WFS_SetDebugMode(s32 a);
BOOL WH_StateInStartScan(void);

BOOL WH_ChildConnectAuto(void *cb, s32 mode, u16 *mac, u16 d) {
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
    WH_ChangeSysState(2);
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
        WFS_InitChild(1, 0, WH_Alloc, 0);
        WFS_SetDebugMode(1);
    }
    if (WH_StateInStartScan())
        return TRUE;
    WH_ChangeSysState(9);
    return FALSE;
}
