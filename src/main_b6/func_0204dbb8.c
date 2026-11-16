#include <nitro/types.h>

typedef struct {
    u16 pad0;
    u16 timer;
} UnkWork_0204dbb8;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0204dbb8 *work;
} UnkTask_0204dbb8;

typedef struct {
    u8 pad0[0x10];
    u32 flags;
} UnkPad_0204dbb8;

extern UnkTask_0204dbb8 *data_022b4574;
extern UnkPad_0204dbb8 data_022b65b4;

void func_020446b0(s32 a);
void func_02043c7c(s32 a, s32 b);
s32 func_02043a58(void);
void func_02037134(void);
void func_02054e4c(void);
s32 func_02044690(void);
s32 func_02044670(void);
void func_0206c784(s32 a, s32 b);

void func_0204dbb8(void) {
    UnkWork_0204dbb8 *w = data_022b4574->work;

    if (w->timer < 500) {
        if (((u16)(((*(vu16 *)0x04000130 | *(vu16 *)0x027fffa8) ^ 0x2fff) & 0x2fff) & 0xc0b) || (data_022b65b4.flags & 0x40))
            w->timer = 500;
    }
    if (w->timer == 0x168)
        func_020446b0(2);
    if (w->timer == 0x170)
        func_020446b0(4);
    if (w->timer == 0x200)
        func_02043c7c(5, 0x100);
    w->timer++;
    if (w->timer > 0x200 && func_02043a58() != 0) {
        func_02037134();
        func_02054e4c();
        return;
    }
    func_0206c784(func_02044690(), func_02044670());
}
