#include <nitro/types.h>

typedef struct {
    u8 id;
    u8 pad1[3];
    s16 period;
} UnkStruct_0233ddd8_Def;

typedef struct {
    u8 pad0[8];
    u8 id;
    u8 pad9;
    u16 timer;
} UnkStruct_0233ddd8_Ent;

typedef struct {
    u8 pad0[0x204];
    u8 bg[2][0x200];
    s16 cnt;
    u8 pad606[2];
    UnkStruct_0233ddd8_Ent ent[2];
} UnkStruct_0233ddd8;

extern UnkStruct_0233ddd8_Def data_ov000_02364db4;

void ZoneFx_RotatePaletteRange(void *bg, UnkStruct_0233ddd8_Def *d);
void ZoneFx_StepPaletteAnim(UnkStruct_0233ddd8_Ent *e, void *bg);
void ZoneFx_QueueExtBgPalettes(void *bg);
void ZoneFx_QueueStdBgPalettes(void *bg);

void ZoneFx_Zone5_Update(UnkStruct_0233ddd8 *w) {
    u8 dirty[2] = {0, 0};
    u8 i;
    UnkStruct_0233ddd8_Ent *e;
    UnkStruct_0233ddd8_Def *d = &data_ov000_02364db4;

    w->cnt++;
    if (w->cnt >= d->period) {
        w->cnt = 0;
        ZoneFx_RotatePaletteRange(w->bg[d->id], d);
        dirty[d->id] = 1;
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
