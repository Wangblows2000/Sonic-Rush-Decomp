#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 cnt;
    u8 pad8[4];
    s32 cur;
    s32 target;
    s32 step;
} UnkSub_02064788;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkSub_02064788 *work;
} UnkSubTask_02064788;

typedef struct {
    u8 pad0[8];
    UnkSubTask_02064788 *sub;
    u8 padc[0x714];
    u16 idx;
} UnkWork_02064788;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02064788 *work;
} UnkTask_02064788;

extern u16 data_022c4db8;
extern UnkTask_02064788 *data_022c4dc4;
extern u16 data_020778b0[];

void func_02063d34(void);

void func_02064788(void) {
    UnkWork_02064788 *w = data_022c4dc4->work;
    UnkSub_02064788 *s;
    s32 d;

    if (data_022c4db8 == 0)
        return;
    w->sub->fn = func_02063d34;
    s = w->sub->work;
    s->cnt = 0;
    s->cur = data_020778b0[w->idx - 1];
    s->target = data_020778b0[w->idx];
    d = s->target - s->cur;
    if (d == 0)
        s->step = 0;
    else if (d > 0)
        s->step = 1;
    else
        s->step = -1;
}
