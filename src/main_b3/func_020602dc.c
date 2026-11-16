#include <nitro/types.h>

typedef struct UnkWork_020602dc {
    void (*fn)(struct UnkWork_020602dc *w);
    u8 pad4[0x18];
    void *unk1C;
    u16 unk20;
    u8 pad22[0xd6];
} UnkWork_020602dc;
typedef struct { u8 pad0[0x10]; UnkWork_020602dc *work; } UnkTask_020602dc;
typedef struct { u8 pad0[0x14]; s32 unk14; } UnkInfo_020602dc;
extern UnkWork_020602dc *data_022c4d9c;
extern UnkTask_020602dc *gTaskCurrent;

UnkTask_020602dc *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_02060220(void);
void func_0206026c(void);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void *func_02060e20(void);
void Task_Destroy(UnkTask_020602dc *t);
void func_0205fd64(UnkWork_020602dc *w);
UnkInfo_020602dc *func_0206047c(void *p);
u32 TalkDemo_GetFlags(s32 a);
void func_0205ff2c(UnkWork_020602dc *w);
void func_02060154(UnkWork_020602dc *w);

#pragma thumb on
void func_020602dc(void) {
    UnkTask_020602dc *t = Task_Create(func_02060220, func_0206026c, 7, 0xeffe, 3, sizeof(UnkWork_020602dc));
    UnkWork_020602dc *w;
    vu16 zero;

    if (t != (UnkTask_020602dc *)func_02006abc(0)) {
        w = t->work;
        data_022c4d9c = w;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_020602dc));
        w->unk1C = func_02060e20();
        if (w->unk1C == NULL) {
            Task_Destroy(gTaskCurrent);
            return;
        }
        func_0205fd64(w);
        w->unk20 = func_0206047c(w->unk1C)->unk14;
        if (TalkDemo_GetFlags(0) & 2)
            w->fn = func_0205ff2c;
        else
            w->fn = func_02060154;
    }
}
#pragma thumb reset
