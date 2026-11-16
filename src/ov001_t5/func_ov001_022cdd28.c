#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x34 - 0x20];
    s32 unk34;
} UnkWork_022cdd28;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cdd28 *work;
} UnkTask_022cdd28;

extern UnkTask_022cdd28 *data_022b4574;
extern u8 data_022503e0[];
extern s32 data_02087ccc;
extern u32 data_02087cb8;

void func_ov001_022cba1c(UnkWork_022cdd28 *w);
void func_020351d8(void *a, s32 b, s32 c);
void func_ov001_022ccddc(void);

#pragma thumb on
void func_ov001_022cdd28(void) {
    UnkWork_022cdd28 *w;

    w = data_022b4574->work;
    w->unk1c = -1;
    w->unk18 = 0;
    w->unk34 = -1;
    func_ov001_022cba1c(w);
    func_020351d8(data_022503e0, 0, 0);
    data_022b4574->func = func_ov001_022ccddc;
    data_02087ccc = 0;
    data_02087cb8 &= ~0x10;
}
#pragma thumb reset
