#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s16 vx;
    s16 vy;
    u8 padC[4];
    s16 timer;
    u16 flags;
    u8 pad14[4];
    u8 *out;
} UnkStruct_0235bd10_P;

typedef struct UnkStruct_0235bd10 UnkStruct_0235bd10;
struct UnkStruct_0235bd10 {
    u32 flags;
    void (*cb4)(UnkStruct_0235bd10 *w, UnkStruct_0235bd10_P *p);
    void (*cb8)(UnkStruct_0235bd10 *w, UnkStruct_0235bd10_P *p);
    s32 (*cbC)(void *a, s32 x, s32 y, s32 b, s32 c, s32 d, s32 e);
    void (*cb10)(UnkStruct_0235bd10_P *p);
    u8 spr0[0x2C];
    u32 spr0Flags;
    u8 pad44[0x5C - 0x44];
    u8 spr1[0x2C];
    u32 spr1Flags;
    u8 pad8C[0xA4 - 0x8C];
    u8 mdl[0x160 - 0xA4];
    u32 mdlFlags;
    u8 pad164[0x1A0 - 0x164];
    UnkStruct_0235bd10_P parts[75];
    u8 count;
    u8 unk9D5;
    u8 nplayers;
};

typedef struct {
    u8 pad[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_0235bd10_G;

typedef struct {
    void *pad;
    void *pad4;
    void *pad8;
    void *padC;
    UnkStruct_0235bd10 *work;
} UnkStruct_0235bd10_T;

extern UnkStruct_0235bd10_T *data_022b4574;
extern UnkStruct_0235bd10_G data_022c4560;
typedef struct {
    u8 pad[0x2A0];
    u8 coll[0xA];
    u16 collFlags;
    u8 pad2AC[0x330 - 0x2AC];
    u32 flags330;
} UnkStruct_0235bd10_Pl;

extern UnkStruct_0235bd10_Pl *data_ov000_0236ac60[];
extern s16 data_ov000_023690e4;

void func_020698bc(void *a, void *b, s32 c);
void func_020316f4(void *p);
void func_0203344c(void *p, s32 a, s32 b);
s32 func_02068e58(s32 x, s32 y, s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov000_02343228(void *p, s32 a);
void func_ov000_0235b2c4(UnkStruct_0235bd10_P *p);
void func_ov000_0235b644(UnkStruct_0235bd10_P *p);

void func_ov000_0235bd10(void) {
    UnkStruct_0235bd10 *w;
    u8 i;
    u8 left;
    BOOL ok;
    UnkStruct_0235bd10_P *p;
    u8 j;
    u16 save;

    ok = TRUE;
    w = data_022b4574->work;

    if (w->count == 0)
        return;

    if (data_022c4560.unk1C == 2) {
        if (data_ov000_0236ac60[0]->flags330 & 0x40000) {
            func_020698bc(w->spr0, w->spr1, ok);
            w->cb10 = func_ov000_0235b2c4;
        } else {
            func_020698bc(w->spr0, w->spr1, 2);
            w->cb10 = func_ov000_0235b644;
        }
    }

    if (w->flags & 1)
        return;

    left = w->count;
    w->spr0Flags &= ~1;
    w->spr1Flags &= ~1;
    w->mdlFlags &= ~1;
    if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2) {
        func_020316f4(w->mdl);
    } else {
        func_0203344c(w->spr0, 0, 0);
        func_0203344c(w->spr1, 0, 0);
    }
    w->spr0Flags |= 1;
    w->spr1Flags |= 1;
    w->mdlFlags |= 1;

    for (i = 0; i < 64; i++) {
        if (left == 0)
            break;
        p = &w->parts[i];
        if (p->timer == 0)
            continue;
        left--;
        if (func_02068e58(p->x, p->y, (p->flags & 0x400) ? 0x10 : 0xC4, 0, 0, 0, 0)) {
            p->timer = 1;
            p->flags &= ~0x20;
            if (p->out != NULL)
                *p->out = p->x >> 8;
        } else if (!(p->flags & 0x100) && p->timer < 0xD8) {
            for (j = 0; j < w->nplayers; j++) {
                save = data_ov000_0236ac60[j]->collFlags;
                data_ov000_0236ac60[j]->collFlags = save & ~0x1000;
                if (w->cbC(((u8 **)data_ov000_0236ac60)[j] + 0x2A0, p->x >> 8, p->y >> 8, -6, -16, 0x14, 0x10)) {
                    p->timer = 1;
                    if (p->flags & 0x1000)
                        w->flags &= ~2;
                    p->flags &= ~0x20;
                    func_ov000_02343228(data_ov000_0236ac60[j], 1);
                    w->cb8(w, p);
                }
                data_ov000_0236ac60[j]->collFlags = save;
            }
        }
        if (!(p->flags & 0x20))
            p->timer--;
        if ((p->flags & 0x2000) && !(w->flags & 4)) {
            p->flags &= ~0x2E00;
            p->vx = 0;
            p->vy = -0x100;
        }
        if (p->timer == 0) {
            w->count--;
            continue;
        }
        if (p->flags & 0x1000)
            ok = FALSE;
        if (!(p->flags & 0x400)) {
            p->x += p->vx;
            if (p->flags & 0x8000)
                p->y -= p->vy;
            else
                p->y += p->vy;
        }
        if (!(p->flags & 0x200))
            p->vy += data_ov000_023690e4;
        w->cb10(p);
        if (p->timer > 0x20 || (p->timer & 2))
            w->cb4(w, p);
    }

    if ((w->flags & 2) && ok) {
        w->unk9D5 = 0;
        w->flags &= ~2;
    }
    w->flags &= ~4;
}
