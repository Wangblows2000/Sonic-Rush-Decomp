#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 zone;
    u8 act;
} UnkState_0205c794;

typedef struct {
    u8 pad0[0x48];
    s32 x;
    s32 y;
} UnkSub_0205c794;

typedef struct {
    u8 pad0[6];
    s8 dx;
    s8 dy;
    u8 pad8[8];
    s32 x;
    s32 y;
    u8 pad18[0x11c];
    UnkSub_0205c794 sub;
    u8 pad184[0xc8];
    s32 unk24c[5];
} UnkObj_0205c794;

typedef struct {
    void (*state)(void *);
    UnkObj_0205c794 *obj;
    u32 pad8;
    s16 t;
} UnkTask_0205c794;

typedef struct {
    u8 pad0[0x4b4];
    s32 unk4b4;
    s32 unk4b8;
    s32 unk4bc;
    s32 unk4c0;
    s32 unk4c4;
    s32 unk4c8;
} UnkWork_0205c794;

typedef struct {
    u8 pad0[0x1c];
    s32 unk1c;
} UnkCam_0205c794;

typedef struct {
    u8 pad0[4];
    u16 angle;
} UnkView_0205c794;

extern UnkState_0205c794 gGameState;
extern s8 data_022c4eec[2];
extern UnkView_0205c794 data_ov000_0236a444;
extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]
static inline s32 FX_MUL(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 FX_MULI(s32 a, s32 b) {
    return (a * b) >> 12;
}

void StageTitleCardPlayer_State_Finish(void *t);
void *Camera_GetPrimary(void);
void Camera_WorldToScreen(void *m, s32 x, s32 y, s16 *ox, s16 *oy);
UnkCam_0205c794 *func_ov001_022f71f4(void);
s32 FX_Div(s32 a, s32 b);

void StageTitleCardPlayer_State_Zoom(UnkWork_0205c794 *w, UnkTask_0205c794 *t) {
    s32 n;
    s16 sx;
    s16 sy;
    s32 v;
    UnkObj_0205c794 *obj;
    s32 s;
    s32 m;
    s32 a;
    s32 h;
    UnkSub_0205c794 *sub;
    u32 i;
    s32 y;
    s32 x;

    t->t += 0x28;
    if (t->t >= 0x1000) {
        t->t = 0x1000;
        t->state = (void (*)(void *))StageTitleCardPlayer_State_Finish;
    }
    obj = t->obj;
    sub = &obj->sub;
    if (gGameState.zone != 8 || gGameState.act != 2) {
        Camera_WorldToScreen(Camera_GetPrimary(), obj->x, obj->y, &sx, &sy);
        x = (t->obj->dx + (sx + data_022c4eec[0])) << 12;
        y = -((sy + data_022c4eec[1]) - t->obj->dy) << 12;
    } else {
        x = obj->x << 12;
        y = -obj->y << 12;
    }
    n = 1;
    do {
        x = FX_MUL(x - 0x80000, t->t) + 0x80000;
    } while (n-- != 0);
    sub->x = x;
    v = -0x60000;
    m = 1;
    do {
        v = v + FX_MUL(y - v, t->t);
    } while (m-- != 0);
    sub->y = v;
    s = (s16)FX_MUL(t->t, 0x1333);
    if (s > 0x1000)
        s = 0x1000;
    if (gGameState.zone != 8 || gGameState.act != 2) {
        a = (data_ov000_0236a444.angle & 0xffff) >> 4;
        h = FX_Div(COS_IDX(a), SIN_IDX(a)) * 0x60;
    } else {
        h = func_ov001_022f71f4()->unk1c;
    }
    w->unk4b4 = 0x80000;
    a = FX_MUL(-0x5000, s) - 0x5b000;
    w->unk4b8 = a;
    w->unk4bc = FX_MUL(h - 0x3c000, s) + 0x3c000;
    w->unk4c0 = 0x80000;
    w->unk4c4 = a;
    w->unk4c8 = 0;
    v = 0x199 + ((t->t * (s32)0xe67) >> 12);
    obj = t->obj;
    for (i = 0; i < 5; i++)
        obj->unk24c[i] = v;
}
