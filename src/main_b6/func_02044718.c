#include <nitro/types.h>

typedef struct { u8 pad[0x10]; void *work; } Task;
extern Task *data_022c4478;
void func_0204440c(void);
void func_020443f8(void);
Task *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
Task *func_02006abc(s32 i);
void MIi_CpuClear16(u16 v, void *p, u32 n);

void func_02044718(void) {
    Task *t = func_020372d0(func_0204440c, func_020443f8, 0, 0x3e00, 3, 8);
    if (t == func_02006abc(0))
        return;
    {
        vu16 z;
        data_022c4478 = t;
        z = 0;
        MIi_CpuClear16(z, t->work, 8);
    }
}
