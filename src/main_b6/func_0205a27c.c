#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    u32 unk14;
    u32 unk18;
} UnkWork_0205a27c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0205a27c *work;
} UnkTask_0205a27c;

void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_0205a23c(void);
void func_0205a278(void);

void func_0205a27c(u32 a, u32 b) {
    UnkTask_0205a27c *t = Task_Create(func_0205a23c, func_0205a278, 0x20, 0, 3, 0x1c);
    UnkWork_0205a27c *w;

    if (t == OS_GetArenaLo(0))
        return;
    w = t->work;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, w, 0x1c);
    }
    w->unk14 = a;
    w->unk18 = b;
}
