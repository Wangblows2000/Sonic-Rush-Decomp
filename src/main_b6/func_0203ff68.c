#include <nitro/types.h>

typedef struct {
    u8 pad0[0x16];
    u16 unk16;
} UnkCfg_0203ff68;

extern s32 data_022c1980;
extern UnkCfg_0203ff68 data_022c1a00;
extern u32 data_022c1964;
extern u8 data_022c1da0[];
extern u32 data_022c1990;
extern u8 data_022c1fc0[];

void func_020405e0(s32 a);
void func_020405bc(s32 v);
s32 func_02011dcc(void (*cb)(void), void *recvBuf, u16 recvBufSize, void *sendBuf, u16 sendBufSize,
                  u16 mpFreq, u16 retry, BOOL a, BOOL b, BOOL c, BOOL d);
void func_0203fdb0(void);

s32 func_0203ff68(void) {
    s32 r;

    if ((u32)(data_022c1980 - 4) <= 2)
        return 1;
    func_020405e0(4);
    r = func_02011dcc(func_0203fdb0, data_022c1fc0, data_022c1990, data_022c1da0, data_022c1964, data_022c1a00.unk16 == 0, 0, 0, 0, 0, 0);
    if (r == 2)
        return 1;
    func_020405bc(r);
    return 0;
}
