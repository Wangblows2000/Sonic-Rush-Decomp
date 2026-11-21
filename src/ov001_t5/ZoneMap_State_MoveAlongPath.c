#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    u8 pad8[4];
    s32 state;
} UnkPos_022e7c50;

typedef struct {
    s32 count;
    s32 state[24];
} UnkGroup_022e7c50;

typedef struct {
    u8 pad0[0x674];
    UnkPos_022e7c50 pos[17];
    UnkGroup_022e7c50 group[1];
} UnkSub_022e7c50;

typedef struct {
    u8 pad0[0x14];
    s32 cur;
    u8 pad18[0x44 - 0x18];
    s32 grp;
    s32 dst;
    s32 src;
    u8 pad50[0x6c - 0x50];
    s32 dispX;
    s32 dispY;
    u32 timer;
    u8 pad78[0xa4 - 0x78];
    UnkSub_022e7c50 *sub;
} UnkWork_022e7c50;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e7c50 *work;
} UnkTask_022e7c50;

extern UnkTask_022e7c50 *gTaskCurrent;

void ZoneMap_CalcClampedScroll(void *a, s32 x, s32 y, s32 *ox, s32 *oy);
void SysSound_PlaySE(s32 se);
void ZoneMap_UpdateView(UnkWork_022e7c50 *w);
void ZoneMap_Action_ScrollToPos(s32 x, s32 y, s32 z);
void ZoneMap_SaveProgressOnPathArrival(UnkWork_022e7c50 *w);

#pragma thumb on
void ZoneMap_State_MoveAlongPath(void) {
    UnkWork_022e7c50 *w;
    UnkSub_022e7c50 *s;
    UnkGroup_022e7c50 *g;
    UnkPos_022e7c50 *from;
    UnkPos_022e7c50 *to;
    s32 i;
    s32 n;
    s32 t;
    s32 base;
    s32 x2;
    s32 y2;
    s32 x;
    s32 y;

    w = gTaskCurrent->work;
    s = w->sub;
    g = &s->group[w->grp];
    from = &s->pos[w->src];
    to = &s->pos[w->dst];
    for (i = 0; i < g->count; i++) {
        if (w->timer >= (i + 1) * 12) {
            if (w->timer / 12 == i + 1) {
                if (g->state[i] != 1) {
                    SysSound_PlaySE(0xe);
                }
                g->state[i] = 1;
            } else {
                g->state[i] = 0;
            }
        } else {
            g->state[i] = 2;
        }
    }
    if (w->timer >= (g->count + 1) * 12) {
        if (to->state != 0) {
            SysSound_PlaySE(0xf);
        }
        to->state = 0;
    } else {
        to->state = 3;
    }
    n = (g->count + 2) * 12;
    t = w->timer;
    if (t > n) t = n;
    base = from->x;
    x = base + (to->x - base) * t / n;
    base = from->y;
    y = base + (to->y - base) * t / n;
    ZoneMap_CalcClampedScroll((u8 *)w->sub + 4, x, y, &x, &y);
    w->dispX = x;
    w->dispY = y;
    ZoneMap_UpdateView(w);
    w->timer++;
    if (w->timer >= n + 30) {
        w->timer = 0;
        x2 = w->sub->pos[w->cur].x;
        y2 = w->sub->pos[w->cur].y;
        ZoneMap_CalcClampedScroll((u8 *)w->sub + 4, x2, y2, &x2, &y2);
        ZoneMap_Action_ScrollToPos(x2, y2, 0x3000);
        ZoneMap_SaveProgressOnPathArrival(w);
    }
}
#pragma thumb reset
