#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s16 t;
    u8 pada[6];
    s32 sel;
} UnkSub_02064674;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkSub_02064674 *work;
} UnkSubTask_02064674;

typedef struct {
    u8 pad0[0xc];
    UnkSubTask_02064674 *sub;
    u8 pad10[0x634];
    s16 x644;
    s16 y646;
    u8 pad648[0x44];
    s16 x68c;
    s16 y68e;
} UnkWork_02064674;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02064674 *work;
} UnkTask_02064674;

typedef struct {
    s16 x;
    s16 y;
} UnkPos_02064674;

extern u16 data_022c4dbc;
extern UnkTask_02064674 *gNaviMTask;
extern UnkPos_02064674 data_02077894[];

void NaviM_Unk_02063b3c(void);

void NaviM_Unk_02064674(void) {
    UnkWork_02064674 *w;
    UnkSub_02064674 *s;
    s32 i;

    if (data_022c4dbc == 0)
        return;
    w = gNaviMTask->work;
    w->sub->fn = NaviM_Unk_02063b3c;
    s = w->sub->work;
    s->unk4 = 0;
    s->t = 0;
    for (i = 0; i < 7; i++) {
        if (data_022c4dbc & (1 << i))
            break;
    }
    s->sel = i;
    w->x68c = data_02077894[s->sel].x;
    w->x644 = w->x68c;
    w->y68e = data_02077894[s->sel].y;
    w->y646 = w->y68e;
}
