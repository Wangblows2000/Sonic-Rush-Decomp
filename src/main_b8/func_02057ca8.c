#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkTask_02057ca8;

UnkTask_02057ca8 *func_020372d0(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02057c68(void);
void func_02057c9c(void);
void func_02057af8(void *w);
void func_0205788c(void *w);

#pragma thumb on
void func_02057ca8(void) {
    UnkTask_02057ca8 *t;
    void *w;
    vu16 z;

    t = func_020372d0(func_02057c68, func_02057c9c, 7, 0x3efe, 3, 0x218);
    if (t == (UnkTask_02057ca8 *)func_02006abc(0))
        return;
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x218);
    func_02057af8(w);
    func_0205788c(w);
}
#pragma thumb reset
