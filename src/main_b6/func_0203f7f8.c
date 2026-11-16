#include <nitro/types.h>

typedef struct {
    void *buf;
    vu16 ch;
    u16 unk6;
} UnkScan_0203f7f8;

extern s32 data_022c1980;
extern u16 data_022c194c;
extern UnkScan_0203f7f8 data_022c19e0;
extern u8 data_022c1ae0[];

void OS_Terminate(void);
u32 func_020111e4(void);
void func_020405bc(s32 v);
u16 func_02010ff4(void);
s32 func_020117b8(void (*cb)(void), UnkScan_0203f7f8 *param);
void func_0203f5a0(void);

s32 func_0203f7f8(void) {
    u32 m;
    s32 r;

    if (data_022c1980 != 2)
        OS_Terminate();
    m = func_020111e4();
    if (m == 0x8000) {
        func_020405bc(3);
        return 0;
    }
    if (m == 0) {
        func_020405bc(0x16);
        return 0;
    }
    if (data_022c194c == 0) {
        do {
            data_022c19e0.ch++;
            if (data_022c19e0.ch > 16)
                data_022c19e0.ch = 1;
        } while ((m & (1 << (data_022c19e0.ch - 1))) == 0);
    } else {
        data_022c19e0.ch = data_022c194c;
    }
    data_022c19e0.unk6 = func_02010ff4();
    data_022c19e0.buf = data_022c1ae0;
    r = func_020117b8(func_0203f5a0, &data_022c19e0);
    if (r == 2)
        return 1;
    func_020405bc(r);
    return 0;
}
