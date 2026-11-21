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

extern UnkTask_02044120 *gTaskCurrent;
extern u16 gPaletteAnimRefCount[];

void Task_Destroy(UnkTask_02044120 *t);
void ObjDraw_LoadPaletteRow(void *pal, u16 col, u8 idx, s32 screen);

void PaletteAnim_Main(void) {
    UnkWork_02044120 *w = gTaskCurrent->work;

    if (gPaletteAnimRefCount[w->idx] == 0) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    if (w->timer == 0) {
        w->frame++;
        w->timer = w->entries[w->frame].time;
        if (w->entries[w->frame].flags & 1) {
            w->frame = 0;
            w->timer = w->entries[w->frame].time;
        }
        ObjDraw_LoadPaletteRow(w->pal, w->entries[w->frame].col, w->idx, 0);
        ObjDraw_LoadPaletteRow(w->pal, w->entries[w->frame].col, w->idx, 1);
        if (w->timer == 0) {
            Task_Destroy(gTaskCurrent);
            return;
        }
    }
    if (w->timer != 0)
        w->timer--;
}
