#include <nitro/types.h>

typedef struct {
    u16 eva : 5;
    u16 pad0 : 3;
    u16 evb : 5;
    u16 pad1 : 3;
} UnkAlpha_022d2cbc;

typedef struct {
    u16 x;
    u16 y;
} UnkOfs_022d2cbc;

typedef struct {
    UnkOfs_022d2cbc ofs[4];
    u8 pad10[0x20 - 0x10];
    u16 blend;
    UnkAlpha_022d2cbc alpha;
} UnkGfx_022d2cbc;

typedef struct {
    void *p;
    u8 pad4[0x44];
} UnkA_022d2cbc;

typedef struct {
    void *p0;
    void *p1;
    u8 pad8[0x74];
} UnkB_022d2cbc;

typedef struct {
    s32 mode;
    u8 pad4[4];
    void *task;
    u8 padc[0x454 - 0xc];
    UnkA_022d2cbc objs[18];
    UnkA_022d2cbc c[2];
    u8 pad9f4[0xa1c - 0x9f4];
    UnkB_022d2cbc b[4];
    u8 padc0c[0xc18 - 0xc0c];
    UnkA_022d2cbc d[3];
} UnkWork_022d2cbc;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d2cbc *work;
} UnkTask_022d2cbc;

extern UnkTask_022d2cbc *data_022b4574;
extern UnkGfx_022d2cbc data_0208922c;
extern UnkGfx_022d2cbc data_02087d04;

void func_020371c8(void *t);
void func_02039bb0(s32 a, void *p);
void func_ov001_022d289c(UnkWork_022d2cbc *w);

#pragma thumb on
void func_ov001_022d2cbc(void) {
    UnkWork_022d2cbc *w;
    s32 i;

    w = data_022b4574->work;
    if (w->task != NULL) {
        func_020371c8(w->task);
        w->task = NULL;
    }
    data_0208922c.blend = 0;
    data_0208922c.alpha.eva = 0;
    data_0208922c.alpha.evb = 0xf;
    data_02087d04.blend = 0;
    data_02087d04.alpha.eva = 0;
    data_02087d04.alpha.evb = 0xf;
    for (i = 0; i < 4; i++) {
        data_0208922c.ofs[i].x = 0;
        data_0208922c.ofs[i].y = 0;
        data_02087d04.ofs[i].x = 0;
        data_02087d04.ofs[i].y = 0;
    }
    for (i = 0; i < 18; i++) {
        func_02039bb0(0, w->objs[i].p);
    }
    func_02039bb0(1, w->c[0].p);
    func_02039bb0(1, w->c[1].p);
    for (i = 0; i < 4; i++) {
        func_02039bb0(0, w->b[i].p0);
        func_02039bb0(1, w->b[i].p1);
    }
    if (w->mode != 2) {
        for (i = 0; i < 3; i++) {
            func_02039bb0(0, w->d[i].p);
        }
    }
    func_ov001_022d289c(w);
}
#pragma thumb reset
