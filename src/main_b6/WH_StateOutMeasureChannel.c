#include <nitro/types.h>

typedef struct {
    u16 pad0;
    u16 unk2;
    u8 pad4[4];
    u16 unk8;
    u16 unka;
} UnkArg_0203e6a0;

extern void (*data_022c197c)(const char *s, ...);
extern char data_02078798[];
extern u16 data_022c1958;
extern u16 data_022c1954;

void WH_SetError(s32 v);
void WH_ChangeSysState(s32 a);
s32 WH_StateInMeasureChannel(u16 ch);

void WH_StateOutMeasureChannel(UnkArg_0203e6a0 *arg) {
    s32 r;
    u16 val;
    u16 ch;

    if (arg->unk2 != 0) {
        WH_SetError(arg->unk2);
        WH_ChangeSysState(9);
        return;
    }
    if (data_022c197c != NULL)
        data_022c197c(data_02078798, arg->unk8, arg->unka);
    val = arg->unka;
    ch = arg->unk8;
    if (data_022c1958 > val) {
        data_022c1958 = val;
        data_022c1954 = 1 << (ch - 1);
    } else if (data_022c1958 == val) {
        data_022c1954 |= 1 << (ch - 1);
    }
    r = WH_StateInMeasureChannel(ch + 1);
    if (r == 0x18) {
        WH_ChangeSysState(7);
        return;
    }
    if (r == 2)
        return;
    WH_ChangeSysState(9);
}
