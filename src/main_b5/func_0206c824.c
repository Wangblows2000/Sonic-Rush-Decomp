#include <nitro/types.h>

extern u8 *data_0224ff44[];
extern u8 data_0207cfb4[];
extern u8 *data_022c4f14;

void func_0206caf0(void);
s32 func_0206c7f0(u8 *p);

u8 *func_0206c824(u8 type, s32 sec) {
    u8 *start;
    u8 *p;

    if (*data_0224ff44[0] != 0x55)
        return NULL;
    func_0206caf0();
    start = data_0224ff44[data_0207cfb4[sec]];
    p = data_022c4f14;
    if (start <= p && data_0224ff44[data_0207cfb4[sec] + 1] > p) {
        p = data_022c4f14 += func_0206c7f0(p);
    } else {
        data_022c4f14 = start;
        p = data_022c4f14 += 4;
    }
    for (;;) {
        if (data_0224ff44[data_0207cfb4[sec] + 1] <= p) {
            data_022c4f14 = NULL;
            return NULL;
        }
        if (*p == type)
            return p;
        if (*p == 0)
            return NULL;
        p = data_022c4f14 += func_0206c7f0(p);
    }
}
