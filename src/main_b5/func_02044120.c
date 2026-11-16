#include <nitro/types.h>

typedef struct {
    u16 col;
    u8 time;
    u8 flags;
} UnkEntry_02044120;

typedef struct {
    void *pal;
    u16 idx;
    u16 frame;
    u8 pad8[2];
    u16 timer;
    UnkEntry_02044120 *entries;
} UnkWork_02044120;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02044120 *work;
} UnkTask_02044120;

extern UnkTask_02044120 *data_022b4574;
extern u16 data_022c4418[];

void func_020371c8(UnkTask_02044120 *t);
void func_0206d190(void *pal, u16 col, u8 idx, s32 screen);

void func_02044120(void) {
    UnkWork_02044120 *w = data_022b4574->work;

    if (data_022c4418[w->idx] == 0) {
        func_020371c8(data_022b4574);
        return;
    }
    if (w->timer == 0) {
        w->frame++;
        w->timer = w->entries[w->frame].time;
        if (w->entries[w->frame].flags & 1) {
            w->frame = 0;
            w->timer = w->entries[w->frame].time;
        }
        func_0206d190(w->pal, w->entries[w->frame].col, w->idx, 0);
        func_0206d190(w->pal, w->entries[w->frame].col, w->idx, 1);
        if (w->timer == 0) {
            func_020371c8(data_022b4574);
            return;
        }
    }
    if (w->timer != 0)
        w->timer--;
}
