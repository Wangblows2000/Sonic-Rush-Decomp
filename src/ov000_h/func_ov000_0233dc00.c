#include <nitro/types.h>

typedef struct {
    u8 id;
    u8 pad1[3];
    s16 period;
} UnkStruct_0233dc00_Def;

typedef struct {
    u8 pad0[8];
    u8 id;
    u8 pad9;
    u16 timer;
} UnkStruct_0233dc00_Ent;

typedef struct {
    u8 bg[2][0x200];
    s16 cnt[2];
    UnkStruct_0233dc00_Ent ent[2];
} UnkStruct_0233dc00;

extern UnkStruct_0233dc00_Def data_ov000_02364dc4[];

void func_ov000_0233c9a4(void *bg, UnkStruct_0233dc00_Def *d);
void func_ov000_0233c8d4(UnkStruct_0233dc00_Ent *e, void *bg);
void func_ov000_0233c878(void *bg);
void func_ov000_0233c840(void *bg);

void func_ov000_0233dc00(UnkStruct_0233dc00 *w) {
    u8 dirty[2] = {0, 0};
    u8 i;
    UnkStruct_0233dc00_Def *d;
    UnkStruct_0233dc00_Ent *e;

    for (i = 0; i < 2; i++) {
        d = &data_ov000_02364dc4[i];
        w->cnt[i]++;
        if (w->cnt[i] >= d->period) {
            w->cnt[i] = 0;
            func_ov000_0233c9a4(w->bg[d->id], d);
            dirty[d->id] = 1;
        }
    }
    for (i = 0; i < 2; i++) {
        e = &w->ent[i];
        e->timer--;
        if (e->timer == 0) {
            func_ov000_0233c8d4(e, w->bg[e->id]);
            dirty[e->id] = 1;
        }
    }
    if (dirty[0])
        func_ov000_0233c878(w->bg[0]);
    if (dirty[1])
        func_ov000_0233c840(w->bg[1]);
}
