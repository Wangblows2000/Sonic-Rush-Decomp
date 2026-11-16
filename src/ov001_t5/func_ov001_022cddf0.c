#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    void *work;
} UnkTask_022cddf0;

extern UnkTask_022cddf0 *data_022b4574;
extern u8 data_022503e0[];

void func_020521b4(void);
void func_ov001_022cb770(void *w);
void func_020351d8(void *a, s32 b, u16 *c);
void func_ov001_022cd05c(void);

#pragma thumb on
void func_ov001_022cddf0(void) {
    void *w;
    s32 i;
    u16 buf[12];

    w = data_022b4574->work;
    func_020521b4();
    func_ov001_022cb770(w);
    for (i = 0; i < 12; i++) {
        buf[i] = 4;
    }
    func_020351d8(data_022503e0, 0, buf);
    data_022b4574->func = func_ov001_022cd05c;
}
#pragma thumb reset
