#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 mode;
    u8 pad8[0xd0 - 8];
    s32 sel;
    u8 padd4[4];
    s32 max;
    u8 paddc[0x10a4 - 0xdc];
} UnkWork_022ec1e4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022ec1e4 *work;
} UnkTask_022ec1e4;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1c;
    u8 unk1d;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022ec1e4;

extern UnkStruct_022c4560 data_022c4560;
extern UnkBright_022ec1e4 data_0208926c;
extern s32 data_02087ccc;
extern u32 data_02087cb8;
extern u8 data_022503e0[];

UnkTask_022ec1e4 *func_020372d0(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov001_022eb5d0(void);
void func_ov001_022ebe98(void);
s32 func_0204e6e8(u8 a);
void func_ov001_022e980c(void);
void func_ov001_022ec16c(UnkWork_022ec1e4 *w);
void func_ov001_022ebfd4(UnkWork_022ec1e4 *w);
void func_ov001_022ebecc(UnkWork_022ec1e4 *w);
void func_ov001_022e9dc8(void);
void func_ov001_022e9ba8(s32 a, s32 b, s32 c, s32 d);
void func_02037d60(void);
void func_ov001_022ebc98(UnkTask_022ec1e4 *t);
void func_020522f4(void);
void func_02052168(s32 a);
void func_020351d8(void *p, s32 a, void *b);

#pragma thumb on
void func_ov001_022ec1e4(s32 mode) {
    UnkTask_022ec1e4 *t;
    UnkWork_022ec1e4 *w;
    vu16 fill;
    vu32 zero;
    u16 buf[12];

    t = func_020372d0(func_ov001_022eb5d0, func_ov001_022ebe98, 0, 0x1000, 0x1000, sizeof(UnkWork_022ec1e4));
    w = t->work;
    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkWork_022ec1e4));
    w->unk0 = 0;
    w->mode = mode;
    w->sel = 0;
    switch (w->mode) {
    case 0:
    case 2:
        w->max = 0xe;
        w->max += 4;
        break;
    case 1:
    case 3:
        w->max = 7;
        break;
    }
    switch (w->mode) {
    case 0:
        w->sel = data_022c4560.unk1d + func_0204e6e8(data_022c4560.unk1c) * 2;
        break;
    case 1:
        w->sel = func_0204e6e8(data_022c4560.unk1c);
        break;
    }
    if (w->sel >= w->max) {
        w->sel = 0;
    }
    func_ov001_022e980c();
    func_ov001_022ec16c(w);
    func_ov001_022ebfd4(w);
    func_ov001_022ebecc(w);
    func_ov001_022e9dc8();
    if (data_0208926c.bright >= 0) {
        data_0208926c.bright = 0x10;
    } else {
        data_0208926c.bright = -0x10;
    }
    func_ov001_022e9ba8(data_0208926c.bright, 0, 8, 0);
    func_02037d60();
    func_ov001_022ebc98(t);
    func_020522f4();
    func_02052168(0);
    data_02087ccc = 0;
    data_02087cb8 &= ~0x10;
    fill = 4;
    MIi_CpuClear16(fill, buf, sizeof(buf));
    func_020351d8(data_022503e0, 0, buf);
}
#pragma thumb reset
