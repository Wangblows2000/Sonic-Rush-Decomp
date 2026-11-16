#include <nitro/types.h>

typedef struct {
    void (*state)(void *);
    u32 pad4;
} UnkWork_02060288;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02060288 *work;
} UnkTask_02060288;

UnkTask_02060288 *Task_Create(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_0205fa74(void);
void func_0205fac0(void);
void func_0205f920(void *t);

#pragma thumb on
void func_02060288(void) {
    UnkTask_02060288 *t;
    UnkWork_02060288 *w;
    vu16 z;

    t = Task_Create(func_0205fa74, func_0205fac0, 7, 0xeffe, 3, 8);
    if (t == (UnkTask_02060288 *)func_02006abc(0))
        return;
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 8);
    w->state = func_0205f920;
}
#pragma thumb reset
