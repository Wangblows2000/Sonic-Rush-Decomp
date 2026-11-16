#include <nitro/types.h>

typedef struct {
    s16 sel;
    s16 max;
    u16 flags;
    u8 pad6[0x15a];
    u16 mark[2];
    void *unk164;
    void *unk168;
    void *unk16c;
    u8 pad170[4];
    u16 held;
    u16 trig;
    u16 rep;
} UnkWork_020556a4;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_020556a4 *work;
} UnkTask_020556a4;

typedef struct {
    u16 held;
    u8 pad2[2];
    u16 trig;
    u8 pad6[2];
    u16 rep;
} UnkPad_020556a4;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022c4560;

extern UnkTask_020556a4 *gTaskCurrent;
extern UnkPad_020556a4 gPad;
extern UnkStruct_022c4560 gGameState;

void Sound_PlaySeqArcEx(s32 a, void *b, void *c);
void func_020551b0(UnkWork_020556a4 *w);
void func_020552bc(UnkWork_020556a4 *w);
void BrightnessFade_Create(s32 a, s32 b);
void func_0205557c(void);
void Task_Destroy(UnkTask_020556a4 *t);

void PauseMenu_Main(void) {
    UnkWork_020556a4 *w = gTaskCurrent->work;
    BOOL moved;

    w->held = gPad.held;
    w->trig = gPad.trig;
    w->rep = gPad.rep;
    if ((w->flags & 2) && (w->trig & 0xc0)) {
        moved = FALSE;
        if (w->rep & 0x40) {
            if (w->sel != 0)
                moved = TRUE;
            w->sel--;
            if (w->sel < 0)
                w->sel = 0;
        }
        if (w->rep & 0x80) {
            if (w->sel == 0)
                moved = TRUE;
            w->sel++;
            if (w->sel >= w->max)
                w->sel = w->max;
        }
        Sound_PlaySeqArcEx(0, w->unk164, w->unk16c);
        w->mark[w->sel] = 2;
        if (moved)
            func_020551b0(w);
    }
    func_020552bc(w);
    if (w->trig & 1) {
        switch (w->sel) {
        case 0:
            w->flags |= 1;
            break;
        case 1:
            if (gGameState.unk10 == 2)
                BrightnessFade_Create(4, 0x100);
            else
                BrightnessFade_Create(5, 0x100);
            gTaskCurrent->fn = func_0205557c;
            return;
        }
    }
    if (!(w->flags & 1) && !(gPad.trig & 0xa))
        return;
    Sound_PlaySeqArcEx(0, w->unk164, w->unk168);
    Task_Destroy(gTaskCurrent);
}
