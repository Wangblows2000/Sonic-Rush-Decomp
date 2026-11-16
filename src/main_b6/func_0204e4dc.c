#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0xa];
    u16 unke;
    u8 pad10[0xc];
    u8 zone;
    u8 act;
} UnkState_0204e4dc;

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
    u8 pade[0x56];
    u16 unk64;
} UnkProg_0204e4dc;

extern UnkState_0204e4dc data_022c4560;
extern UnkProg_0204e4dc data_022c4680;
extern u16 data_022c45f8;
extern u8 data_02074bbc[];
extern u8 data_02074bb4[];

u32 func_0204e6e8(u8 zone);
u8 func_0204e6e4(u8 act);
BOOL func_0204eb7c(void);
void func_02059bdc(u8 v);
void func_02059bcc(u8 v);
u16 func_0204e614(u16 v);

void func_0204e4dc(void) {
    u32 z = func_0204e6e8(data_022c4560.zone);
    u8 a = func_0204e6e4(data_022c4560.act);

    if (z >= 7)
        return;
    if (a == 0) {
        if (func_0204eb7c())
            data_022c45f8 |= 1;
        return;
    }
    if (func_0204eb7c()) {
        data_022c45f8 |= 1;
        if (data_022c4560.unk0 != 1)
            func_02059bdc(a != 2 ? data_02074bbc[z] - 1 : data_02074bbc[z]);
        else
            func_02059bcc(a != 2 ? data_02074bb4[z] - 1 : data_02074bb4[z]);
    }
    if (data_022c4560.unk0 != 1)
        data_022c4560.unke = func_0204e614(data_022c4680.unkc);
    else
        data_022c4560.unke = func_0204e614(data_022c4680.unk64);
}
