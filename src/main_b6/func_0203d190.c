#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padc[0x1c];
    u32 unk28;
    u32 unk2c;
    u16 unk30;
    u8 pad32[0xe];
    u8 buf40[0x10];
} UnkCtx_0203d190;

extern UnkCtx_0203d190 data_022b8be0;

u32 func_02014b74(void *a, u16 b);
u32 func_02014adc(void *a, u16 b);
s32 func_02011c44(void (*cb)(void), void *arg, void *data, u16 size, u16 dest, u16 port, u16 prio);
void func_0203d150(void);

void func_0203d190(void) {
    UnkCtx_0203d190 *c = &data_022b8be0;
    u16 len;
    u16 sz;

    s32 mode = c->unk0;
    void *buf;

    if (mode != 0)
        len = c->unk28;
    else
        len = c->unk2c;
    if (c->unk4 == 0)
        return;
    buf = c->buf40;
    if (mode != 0)
        sz = func_02014b74(buf, len);
    else
        sz = func_02014adc(buf, len);
    c->unk8 = func_02011c44(func_0203d150, NULL, buf, sz, 0xffff, c->unk30, 3) == 2;
    c->unk4 = data_022b8be0.unk8 == 0;
}
