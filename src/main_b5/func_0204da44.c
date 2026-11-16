#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[4];
    s32 unk1c;
    s32 unk20;
    u8 pad24[0xa];
    u16 unk2e;
    u8 pad30[0x24];
    u32 unk54;
    u32 unk58;
    u16 unk5c;
    u8 pad5e[0xa];
    void (*unk68)(void);
} UnkWork_0204da44;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0204da44 *work;
} UnkTask_0204da44;

extern u8 data_0207a1bc[];
extern u8 data_022c45f0[];

UnkTask_0204da44 *func_020372d0(void (*fn)(void), void (*fn2)(void), s32 a, s32 b, s32 c, s32 size);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02069f1c(UnkWork_0204da44 *w, void *a, u32 b, void *c, s32 d);
void func_0206a054(UnkWork_0204da44 *w, s32 a, s32 b);
void func_02069818(UnkWork_0204da44 *w, s32 a);
void func_02069918(UnkWork_0204da44 *w, s32 a);
void func_020698f0(UnkWork_0204da44 *w, s32 a);
void func_0206c3d0(void);
void func_0206bef4(void);
void func_0204d854(void);

void func_0204da44(s32 a, s32 b, s32 x, s32 y, u16 d, s16 e) {
    UnkTask_0204da44 *t;
    UnkWork_0204da44 *w;
    vu16 z;

    t = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, 0x134);
    if (t == (UnkTask_0204da44 *)func_02006abc(0))
        return;
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x134);
    func_02069f1c(w, data_0207a1bc, 0xffff, data_022c45f0, 0);
    func_0206a054(w, 0, 1);
    func_02069818(w, a);
    func_02069918(w, 9);
    func_020698f0(w, 1);
    w->unk5c |= 1;
    w->x = x << 8;
    w->y = y << 8;
    w->unk20 = b;
    w->unk1c = (w->unk20 << 8) + (e << 8);
    w->unk2e = d;
    w->unk58 |= 0x100;
    w->unk58 |= 0x2000;
    w->unk5c |= 0x100;
    w->unk5c |= 0x80;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = func_0204d854;
}
