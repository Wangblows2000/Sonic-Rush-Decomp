#include <nitro/types.h>

typedef struct {
    u8 pad0[5];
    u8 angle;
    u8 pad6[0xa];
    s32 x;
    s32 y;
    s32 z;
    u8 pad1c[0x38];
    u32 flags54;
    u8 pad58[4];
    u16 flags5c;
    u8 pad5e[0x46];
    void *unka4;
} UnkObj_02069090;

typedef struct {
    s32 x;
    s32 y;
    s16 z;
    u16 flags;
    u8 padc[0xc];
    UnkObj_02069090 *owner;
} UnkSpr_02069090;

void ObjRect_Register(UnkSpr_02069090 *s);

void StageObj_HandleCollider(UnkObj_02069090 *o, UnkSpr_02069090 *s) {
    u16 *f;

    if (o->flags54 & 0xc)
        return;
    s->owner = o;
    s->x = o->x >> 8;
    s->y = o->y >> 8;
    s->z = o->z >> 8;
    if (o->flags54 & 2) {
        s->flags |= 0x1000;
        return;
    }
    f = &s->flags;
    s->flags &= ~0x1000;
    s->flags &= ~3;
    if (o->unka4 != NULL) {
        if (!(o->flags5c & 1))
            *f |= 1;
    } else {
        if (!(o->flags5c & 1))
            *f |= 1;
    }
    if (o->angle >= 0x60 && o->angle <= 0xa0)
        s->flags ^= 1;
    if ((o->flags5c & 2) || o->angle == 0x80)
        s->flags |= 2;
    ObjRect_Register(s);
}
