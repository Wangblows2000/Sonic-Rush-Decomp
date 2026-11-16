#include <nitro/types.h>

typedef struct UnkObj_0206919c UnkObj_0206919c;

typedef struct {
    u32 unk0;
    UnkObj_0206919c *a;
    UnkObj_0206919c *b;
} UnkLink_0206919c;

struct UnkObj_0206919c {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    s32 z;
    s32 px;
    s32 py;
    s32 pz;
    s16 vx;
    s16 vy;
    s16 vz;
    u8 pad2e[8];
    s16 unk36;
    s16 unk38;
    s16 unk3a;
    u8 pad3c[0xc];
    s16 max48;
    u8 pad4a[0xa];
    u32 unk54;
    u32 flags;
    u8 pad5c[0x24];
    UnkObj_0206919c *unk80;
    UnkObj_0206919c *unk84;
    u8 pad88[0xa8];
    UnkLink_0206919c *unk130;
};

void func_0206919c(UnkObj_0206919c *o) {
    UnkObj_0206919c *t;
    s32 v;
    s32 lo;

    if (o->unk130 != NULL) {
        if (o->unk130->a != NULL && (o->unk130->a->unk54 & 4))
            o->unk130->a = NULL;
        t = o->unk130->b;
        if (t != NULL && t != o->unk130->a) {
            if ((t->flags & 0x2000000) && (o->flags & 0x4000000)) {
                v = (t->vx < -t->max48) ? -t->max48 : ((t->vx > t->max48) ? t->max48 : t->vx);
                v = ((s16)v < -o->max48) ? -o->max48 : (((s16)v > o->max48) ? o->max48 : (s16)v);
                o->unk36 += (s16)v;
            }
            if (o->unk130->b->unk54 & 4)
                o->unk130->b = NULL;
        }
    }
    if (o->unk84 != NULL) {
        if ((o->unk84->flags & 0x4000000) && (o->flags & 0x2000000)) {
            o->unk36 += (s16)(o->unk84->x - o->unk84->px);
            v = o->unk84->vx;
            lo = v & 0xff;
            if (lo != 0) {
                if (v > 0)
                    o->unk36 += 0x100 - lo;
                else
                    o->unk36 -= lo + 0x100;
            }
        }
        if (o->unk84->unk54 & 4)
            o->unk84 = NULL;
    }
    if (o->unk80 == NULL)
        return;
    if (o->unk80->unk54 & 4) {
        o->unk80 = NULL;
        return;
    }
    if (o->unk80->flags & 0x100) {
        o->unk36 += o->unk80->vx;
        o->unk38 += o->unk80->vy;
        o->unk3a += o->unk80->vz;
    } else {
        o->unk36 += (s16)(o->unk80->x - o->unk80->px);
        o->unk38 += (s16)(o->unk80->y - o->unk80->py);
        o->unk3a += (s16)(o->unk80->z - o->unk80->pz);
    }
    lo = o->unk80->vy & 0xff;
    if (lo != 0)
        o->unk38 += 0x100 - lo;
}
