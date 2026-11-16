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

extern UnkTask_020556a4 *data_022b4574;
extern UnkPad_020556a4 data_022503e0;
extern UnkStruct_022c4560 data_022c4560;

void func_0204b7ec(s32 a, void *b, void *c);
void func_020551b0(UnkWork_020556a4 *w);
void func_020552bc(UnkWork_020556a4 *w);
void func_02043c7c(s32 a, s32 b);
void func_0205557c(void);
void func_020371c8(UnkTask_020556a4 *t);

void func_020556a4(void) {
    UnkWork_020556a4 *w = data_022b4574->work;
    BOOL moved;

    w->held = data_022503e0.held;
    w->trig = data_022503e0.trig;
    w->rep = data_022503e0.rep;
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
        func_0204b7ec(0, w->unk164, w->unk16c);
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
            if (data_022c4560.unk10 == 2)
                func_02043c7c(4, 0x100);
            else
                func_02043c7c(5, 0x100);
            data_022b4574->fn = func_0205557c;
            return;
        }
    }
    if (!(w->flags & 1) && !(data_022503e0.trig & 0xa))
        return;
    func_0204b7ec(0, w->unk164, w->unk168);
    func_020371c8(data_022b4574);
}
