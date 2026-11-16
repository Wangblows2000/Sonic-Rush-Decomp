#include <nitro/types.h>

typedef struct { u8 pad[0x10]; void *work; } UnkStruct_022c66e8;
UnkStruct_022c66e8 *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void MI_CpuFill8(void *dest, u8 data, u32 size);
void ExtraEnding_Main(void);
void ExtraEnding_Destructor(void);
void func_ov001_022c6110(void);
void ExtraEnding_SetState(void *p, void (*f)(void));
void func_ov001_022c634c(void *p);
void ExtraEnding_LoadSound(void *p);
void func_ov001_022c61c4(void *p);

void ExtraEnding_Create(void) {
    void *w;
    *(vu32 *)0x04000000 &= ~0x1f00;
    *(vu32 *)0x04001000 &= ~0x1f00;
    w = Task_Create(ExtraEnding_Main, ExtraEnding_Destructor, 0, 0x5000, 0x1173, 0xa0)->work;
    MI_CpuFill8(w, 0, 0xa0);
    ExtraEnding_SetState(w, func_ov001_022c6110);
    func_ov001_022c634c(w);
    ExtraEnding_LoadSound(w);
    func_ov001_022c61c4(w);
}
