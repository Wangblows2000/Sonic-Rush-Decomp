#include <nitro/types.h>

typedef struct { s32 m[9]; } UnkMtx33_02067a28;
typedef struct { s32 x; s32 y; s32 z; } UnkVec_02067a28;

typedef struct {
    u8 unk0;
    u8 rot;
    u8 pad2[0xe];
    s32 x;
    s32 y;
} UnkObj_02067a28;

typedef struct {
    UnkObj_02067a28 *obj;
    u8 pad4[8];
    u16 flags;
    u8 pade[0x12];
    u16 w;
    u16 h;
} UnkLayer_02067a28;

extern u8 data_022c4ddc;
extern UnkLayer_02067a28 *data_022c4e64[];
extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void ObjCollisionMap_GetOffset(UnkLayer_02067a28 *l, s16 *x, s16 *y);
void MTX_RotZ33_(UnkMtx33_02067a28 *m, s32 s, s32 c);
void MTX_MultVec33(UnkVec_02067a28 *v, UnkMtx33_02067a28 *m, UnkVec_02067a28 *dst);
s32 ObjCollisionMap_ProbeEntry(UnkLayer_02067a28 *l, s32 x, s32 y, u8 layer, u8 dir, u8 *a, u8 *b);

static inline void VecSet_02067a28(UnkVec_02067a28 *v, s32 x, s32 y, s32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

s32 ObjCollisionMap_ProbeDistance(s32 x, s32 y, u8 layer, u8 dir, u8 *a, u8 *b) {
    s32 best;
    UnkLayer_02067a28 *l;
    u8 i;
    s32 h;
    s32 sx;
    s32 sy;
    s32 dx;
    s32 dy;
    s32 px;
    s32 py;
    s16 ox;
    s16 oy;
    UnkMtx33_02067a28 m;
    UnkVec_02067a28 v;

    i = 0;
    sx = x;
    sy = y;
    ox = 0;
    oy = 0;
    best = 0x18;
    if (data_022c4ddc == 0)
        return best;
    i = 0;
    if (i < data_022c4ddc) {
        px = sx << 8;
        py = sy << 8;
        do {
            l = data_022c4e64[i];
            if (l->obj == NULL || l->w == 0 || l->h == 0)
                continue;
            x = sx;
            y = sy;
            ObjCollisionMap_GetOffset(l, &ox, &oy);
            if ((l->flags & 1) && l->obj->rot != 0) {
                dx = px - l->obj->x;
                dy = py - l->obj->y;
                v.x = dx << 4;
                v.y = dy << 4;
                v.z = 0;
                MTX_RotZ33_(&m, SIN((u16)-(l->obj->rot << 8)), COS((u16)-(l->obj->rot << 8)));
                MTX_MultVec33(&v, &m, &v);
                x = ((v.x >> 4) + l->obj->x) >> 8;
                y = ((v.y >> 4) + l->obj->y) >> 8;
            }
            if (x < (l->obj->x >> 8) - 0x18 + ox)
                continue;
            if (x > ox + ((l->obj->x >> 8) + (l->w << 3) + 0x18))
                continue;
            if (y < (l->obj->y >> 8) - 0x18 + oy)
                continue;
            if (y > oy + ((l->obj->y >> 8) + (l->h << 3) + 0x18))
                continue;
            h = ObjCollisionMap_ProbeEntry(l, x, y, layer, dir, a, b);
            if (best > h)
                best = h;
        } while (++i < data_022c4ddc);
    }
    return best;
}
