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

void ZoneFx_RotatePaletteRange(void *bg, UnkStruct_0233dc00_Def *d);
void ZoneFx_StepPaletteAnim(UnkStruct_0233dc00_Ent *e, void *bg);
void ZoneFx_QueueExtBgPalettes(void *bg);
void ZoneFx_QueueStdBgPalettes(void *bg);

void ZoneFx_Zone7_Update(UnkStruct_0233dc00 *w) {
    u8 dirty[2] = {0, 0};
    u8 i;
    UnkStruct_0233dc00_Def *d;
    UnkStruct_0233dc00_Ent *e;

    for (i = 0; i < 2; i++) {
        d = &data_ov000_02364dc4[i];
        w->cnt[i]++;
        if (w->cnt[i] >= d->period) {
            w->cnt[i] = 0;
            ZoneFx_RotatePaletteRange(w->bg[d->id], d);
            dirty[d->id] = 1;
        }
    }
    for (i = 0; i < 2; i++) {
        e = &w->ent[i];
        e->timer--;
        if (e->timer == 0) {
            ZoneFx_StepPaletteAnim(e, w->bg[e->id]);
            dirty[e->id] = 1;
        }
    }
    if (dirty[0])
        ZoneFx_QueueExtBgPalettes(w->bg[0]);
    if (dirty[1])
        ZoneFx_QueueStdBgPalettes(w->bg[1]);
}
