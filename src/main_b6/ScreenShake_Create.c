#include <nitro/types.h>

typedef struct { u8 pad[0x10]; void *work; } Task;
extern Task *gScreenShakeTask;
void ScreenShake_Main(void);
void ScreenShake_Destructor(void);
Task *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
Task *func_02006abc(s32 i);
void MIi_CpuClear16(u16 v, void *p, u32 n);

void ScreenShake_Create(void) {
    Task *t = Task_Create(ScreenShake_Main, ScreenShake_Destructor, 0, 0x3e00, 3, 8);
    if (t == func_02006abc(0))
        return;
    {
        vu16 z;
        gScreenShakeTask = t;
        z = 0;
        MIi_CpuClear16(z, t->work, 8);
    }
}
