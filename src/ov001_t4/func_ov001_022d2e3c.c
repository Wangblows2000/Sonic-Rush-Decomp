#include <nitro/types.h>

typedef struct {
    u8 pad0[0x40];
} UnkBg_022d2e3c;

typedef struct {
    s32 mode;
    u8 pad4[4];
    void *task8;
    u8 unkc[0x48];
    UnkBg_022d2e3c bg54;
    UnkBg_022d2e3c bg94;
    u8 padd4[0x80];
    UnkBg_022d2e3c bg154;
    UnkBg_022d2e3c bg194;
    UnkBg_022d2e3c bg1d4;
    UnkBg_022d2e3c bg214;
    UnkBg_022d2e3c bg254;
    UnkBg_022d2e3c bg294;
    u8 pad2d4[0xbb0 - 0x2d4];
    s32 unkbb0;
    u8 padbb4[0x10];
    s32 unkbc4;
} UnkWork_022d2e3c;

typedef struct {
    u8 pad0[0x182];
    u16 unk182;
} UnkGlobal_022d2e3c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d2e3c *work;
} UnkTask_022d2e3c;

typedef struct {
    u8 pad0[8];
    u32 flags;
    u16 unkc;
} UnkSave_022d2e3c;

typedef struct {
    u8 pad0[0x24];
    u16 unk24;
} UnkSave2_022d2e3c;

extern UnkGlobal_022d2e3c *data_ov001_0231f050;
extern UnkSave_022d2e3c data_022c4680;
extern UnkSave2_022d2e3c data_022c46c0;

UnkTask_022d2e3c *func_020372d0(void (*func)(void), void (*dtor)(void), s32 a, s32 b, s32 c, s32 size);
void MI_CpuFill8(void *dst, s32 v, u32 size);
void func_02037d60(void);
void func_ov001_022d1404(UnkWork_022d2e3c *w, void (*f)(void));
void func_ov001_022d2234(UnkWork_022d2e3c *w, void (*f)(void));
void func_ov001_022d2a18(UnkWork_022d2e3c *w);
void func_ov001_022d2978(UnkWork_022d2e3c *w);
void func_ov001_022d2704(UnkWork_022d2e3c *w);
void func_0203438c(void *bg);
void func_02020230(void *p, s32 a, s32 b);
void func_ov001_022d2460(void);
void func_ov001_022d2cbc(void);
void func_ov001_022d13f4(void);
void func_ov001_022d0d34(void);
void func_ov001_022d2200(void);
void func_ov001_022d224c(void);

#pragma thumb on
void func_ov001_022d2e3c(s32 mode) {
    UnkWork_022d2e3c *w;

    *(vu32 *)0x4000000 = *(vu32 *)0x4000000 & ~0x1f00;
    *(vu32 *)0x4001000 = *(vu32 *)0x4001000 & ~0x1f00;
    w = func_020372d0(func_ov001_022d2460, func_ov001_022d2cbc, 0, 0x5000, 0x1173, 0x150c)->work;
    MI_CpuFill8(w, 0, 0x150c);
    data_ov001_0231f050 = (UnkGlobal_022d2e3c *)w;
    func_02037d60();
    w->mode = mode;
    switch (w->mode) {
    case 0:
        if (data_022c4680.unkc >= 0x1a) {
            w->unkbb0 = 1;
        }
        func_ov001_022d1404(w, func_ov001_022d13f4);
        break;
    case 1:
        if (data_022c46c0.unk24 >= 0x1c) {
            w->unkbb0 = 1;
        }
        func_ov001_022d1404(w, func_ov001_022d0d34);
        break;
    case 2:
        if (data_022c4680.flags & 0x1000) {
            w->unkbb0 = 1;
        }
        func_ov001_022d1404(w, NULL);
        break;
    }
    func_ov001_022d2234(w, func_ov001_022d2200);
    func_ov001_022d2a18(w);
    func_ov001_022d2978(w);
    func_ov001_022d2704(w);
    w->unkbc4 = data_ov001_0231f050->unk182 * 8 - 0x1d0;
    w->task8 = func_020372d0(func_ov001_022d224c, NULL, 0, 0xf100, 0x1173, 0);
    func_0203438c(&w->bg54);
    func_0203438c(&w->bg154);
    func_0203438c(&w->bg194);
    func_0203438c(&w->bg1d4);
    func_0203438c(&w->bg94);
    func_0203438c(&w->bg214);
    func_0203438c(&w->bg254);
    func_0203438c(&w->bg294);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1f00;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1f00;
    if (w->mode == 2) {
        func_02020230(w->unkc, 1, 0);
        return;
    }
    func_02020230(w->unkc, 0, 0);
}
#pragma thumb reset
