#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkTask_02057ca8;

UnkTask_02057ca8 *Task_Create(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void PolyDemoUnder2_Main(void);
void PolyDemoUnder2_Destructor(void);
void PolyDemoUnder2_LoadBg(void *w);
void PolyDemoUnder2_LoadSprites(void *w);

#pragma thumb on
void PolyDemoUnder2_Create(void) {
    UnkTask_02057ca8 *t;
    void *w;
    vu16 z;

    t = Task_Create(PolyDemoUnder2_Main, PolyDemoUnder2_Destructor, 7, 0x3efe, 3, 0x218);
    if (t == (UnkTask_02057ca8 *)func_02006abc(0))
        return;
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x218);
    PolyDemoUnder2_LoadBg(w);
    PolyDemoUnder2_LoadSprites(w);
}
#pragma thumb reset
