#ifndef MAIN_SDK2_TASK_H
#define MAIN_SDK2_TASK_H
#include <nitro/types.h>

typedef struct UnkTask_0201a2e8 {
    struct UnkTask_0201a2e8 *next; /* 0x0 */
    u32 busy : 1;                  /* 0x4 */
    u32 priority : 31;
    void (*func)(struct UnkTask_0201a2e8 *); /* 0x8 */
    void (*done)(struct UnkTask_0201a2e8 *); /* 0xc */
} UnkTask_0201a2e8;

typedef struct {
    u8 thread[0x94];
    UnkTask_0201a2e8 *volatile list; /* 0x94 */
    UnkTask_0201a2e8 idle;         /* 0x98 */
    u8 padA8[0x10];
} UnkTaskCtx_0201a2e8;

extern UnkTaskCtx_0201a2e8 *data_02080f40;
#endif
