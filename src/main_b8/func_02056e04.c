#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    void *unk10;
} UnkRes_02056e04;

typedef struct {
    void (*state)(void);
    UnkRes_02056e04 *res;
    void *unk8;
} UnkWork_02056e04;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02056e04 *work;
} UnkTask_02056e04;

extern UnkTask_02056e04 *gTaskCurrent;

UnkTask_02056e04 *Task_Create(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
UnkRes_02056e04 *func_02058350(void);
void Task_Destroy(UnkTask_02056e04 *t);
void func_02056dc8(void);
void func_02056e00(void);
void func_02056d8c(void);

#pragma thumb on
void func_02056e04(void) {
    UnkTask_02056e04 *t;
    UnkWork_02056e04 *w;
    vu16 z;

    t = Task_Create(func_02056dc8, func_02056e00, 0, 0x3efe, 3, 0x58);
    if (t == (UnkTask_02056e04 *)func_02006abc(0))
        return;
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x58);
    w->res = func_02058350();
    if (w->res == NULL) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    w->unk8 = w->res->unk10;
    w->state = func_02056d8c;
}
#pragma thumb reset
