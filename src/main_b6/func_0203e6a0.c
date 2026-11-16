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

void func_020405bc(s32 v);
void func_020405e0(s32 a);
s32 func_0203e790(u16 ch);

void func_0203e6a0(UnkArg_0203e6a0 *arg) {
    s32 r;
    u16 val;
    u16 ch;

    if (arg->unk2 != 0) {
        func_020405bc(arg->unk2);
        func_020405e0(9);
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
    r = func_0203e790(ch + 1);
    if (r == 0x18) {
        func_020405e0(7);
        return;
    }
    if (r == 2)
        return;
    func_020405e0(9);
}
