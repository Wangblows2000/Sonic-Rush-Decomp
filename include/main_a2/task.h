#ifndef MAIN_A2_TASK_H
#define MAIN_A2_TASK_H

#include <nitro/types.h>

typedef struct UnkTask {
    struct UnkTask *prev;
    struct UnkTask *next;
    void (*func)(void);
    void (*dtor)(struct UnkTask *t);
    void *work;
    u16 flags;
    u16 prio;
    u16 id;
    u16 unk1a;
} UnkTask;

#endif
