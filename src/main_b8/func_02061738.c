#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    void *arg;
} UnkWork_02061738;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02061738 *work;
} UnkTask_02061738;

UnkTask_02061738 *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02061700(void);
void func_0206151c(UnkWork_02061738 *w);

#pragma thumb on
void func_02061738(void *arg) {
    UnkWork_02061738 *w;
    vu16 z;

    w = Task_Create(func_02061700, NULL, 7, 0xeffe, 3, 0x160)->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x160);
    w->arg = arg;
    func_0206151c(w);
}
#pragma thumb reset
