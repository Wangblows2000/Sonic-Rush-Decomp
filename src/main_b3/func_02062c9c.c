#include <nitro/types.h>

typedef struct UnkWork_02062c9c {
    void (*fn)(struct UnkWork_02062c9c *w);
    u8 pad4[0x64];
    void *unk68;
    u8 pad6C[4];
    s32 unk70;
    u8 pad74[4];
    s32 unk78;
    u8 pad7C[0x64];
} UnkWork_02062c9c;
typedef struct { u8 pad0[0x10]; UnkWork_02062c9c *work; } UnkTask_02062c9c;
typedef struct { u8 pad0[8]; s32 unk8; u8 padC[4]; s32 unk10; } UnkInfo_02062c9c;
extern UnkTask_02062c9c *data_022c4da8;
extern UnkTask_02062c9c *gTaskCurrent;

UnkTask_02062c9c *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_02062bb0(void);
void func_02062c4c(void);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void *func_02060e20(void);
void Task_Destroy(UnkTask_02062c9c *t);
void func_020623e0(void);
UnkInfo_02062c9c *func_0206047c(void *p);
u32 TalkDemo_GetFlags(s32 a);
void func_0206243c(UnkWork_02062c9c *w);
void func_02062b7c(UnkWork_02062c9c *w);

#pragma thumb on
void func_02062c9c(void) {
    UnkTask_02062c9c *t;
    UnkWork_02062c9c *w;
    UnkInfo_02062c9c *info;
    vu16 zero;

    t = Task_Create(func_02062bb0, func_02062c4c, 7, 0xeffe, 3, sizeof(UnkWork_02062c9c));
    data_022c4da8 = t;
    if (t != (UnkTask_02062c9c *)func_02006abc(0)) {
        w = t->work;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_02062c9c));
        w->unk68 = func_02060e20();
        if (w->unk68 == NULL) {
            Task_Destroy(gTaskCurrent);
            return;
        }
        info = func_0206047c(w->unk68);
        w->unk70 = info->unk8;
        w->unk78 = info->unk10;
        func_020623e0();
        if (TalkDemo_GetFlags(0) & 2)
            w->fn = func_0206243c;
        else
            w->fn = func_02062b7c;
    }
}
#pragma thumb reset
