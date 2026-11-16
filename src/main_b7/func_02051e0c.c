#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 from;
    s32 to;
    s32 delay;
    s32 duration;
} UnkWork_02051e0c;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_02051e0c *work;
} UnkTask_02051e0c;

typedef struct {
    u8 pad0[0x10];
    UnkTask_02051e0c *task;
} UnkState_02051e0c;

extern UnkState_02051e0c *data_022c465c;
void func_020371c8(UnkTask_02051e0c *t);
UnkTask_02051e0c *func_020372d0(void (*fn)(void), void (*dtor)(void), s32 b, u16 c, u16 d, s32 e);
void func_02051950(void);
void func_02051928(void);
void func_02051e90(u8 v);

#pragma thumb on
void func_02051e0c(s32 from, s32 to, s32 delay, s32 duration, u16 prio, u16 id) {
    UnkWork_02051e0c *w;
    if (data_022c465c == NULL)
        return;
    if (data_022c465c->task != NULL) {
        func_020371c8(data_022c465c->task);
        data_022c465c->task = NULL;
    }
    data_022c465c->task = func_020372d0(func_02051950, func_02051928, 0, prio, id, 0x14);
    w = data_022c465c->task->work;
    w->timer = 0;
    w->from = from;
    w->to = to;
    w->delay = delay;
    w->duration = duration;
    func_02051e90(from);
}
#pragma thumb reset
