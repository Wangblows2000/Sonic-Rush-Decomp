#ifndef MAIN_SDK2_OSTHREAD_H
#define MAIN_SDK2_OSTHREAD_H
#include <nitro/types.h>

typedef struct UnkOSThread {
    u32 cpsr;        /* 0x00 */
    u32 r[13];       /* 0x04 */
    u32 sp;          /* 0x38 */
    u32 lr;          /* 0x3c */
    u8 context40[0x64 - 0x40];
    u32 state;       /* 0x64 */
    struct UnkOSThread *next; /* 0x68 */
    u32 id;          /* 0x6c */
    u32 priority;    /* 0x70 */
    void *profiler;  /* 0x74 */
    void *mutex;     /* 0x78 */
    void *mutexHead; /* 0x7c */
    void *mutexTail; /* 0x80 */
    u32 stackTop;    /* 0x84 */
    u32 stackBottom; /* 0x88 */
    u32 stackWarn;   /* 0x8c */
    u16 joinQueue;   /* 0x90 */
} UnkOSThread;

typedef struct {
    u16 isNeedRescheduling; /* 0x0 */
    u16 pad2;
    u16 irqDepth;        /* 0x4 */
    u16 pad6;
    UnkOSThread *current; /* 0x8 */
    UnkOSThread *list;   /* 0xc */
    void (*switchCallback)(UnkOSThread *from, UnkOSThread *to); /* 0x10 */
} UnkOSThreadInfo2;

extern UnkOSThreadInfo2 data_0207d090;
extern UnkOSThread data_0207d0e4;

typedef struct {
    UnkOSThread *current;
} UnkOSThreadInfo;

extern UnkOSThreadInfo *data_0207d084;
extern UnkOSThread *data_0207d0a4[];

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
#endif
