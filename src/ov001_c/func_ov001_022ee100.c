#include <nitro/types.h>

typedef struct UnkStruct_022ee100_v {
    s32 x, y, z;
} UnkStruct_022ee100_v;

typedef struct UnkStruct_022ee100_m {
    s32 m[9];
} UnkStruct_022ee100_m;

typedef struct UnkStruct_022ee100_n {
    s32 x;
    u8 pad4[8];
    s32 y;
    u8 pad10[8];
    struct UnkStruct_022ee100_n *next;
} UnkStruct_022ee100_n;

typedef struct UnkStruct_022ee100_l {
    u8 pad0[0x18];
    UnkStruct_022ee100_n *head;
} UnkStruct_022ee100_l;

typedef struct UnkStruct_022ee100_s {
    s32 w;
    u8 pad4[0x2a4 - 4];
    UnkStruct_022ee100_l *list;
    s32 count;
} UnkStruct_022ee100_s;

typedef struct UnkStruct_022ee100_o {
    u8 pad0[0x18];
    UnkStruct_022ee100_v pos;
    UnkStruct_022ee100_v prev;
    UnkStruct_022ee100_v ofs;
} UnkStruct_022ee100_o;

typedef struct UnkStruct_022ee100_p {
    u8 pad0[0x7c];
    UnkStruct_022ee100_o *unk7C;
    u8 pad80[0x1dc - 0x80];
    UnkStruct_022ee100_s *unk1DC;
} UnkStruct_022ee100_p;

extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

u16 FX_Atan2(s32 y, s32 x);
void MTX_Identity33_(UnkStruct_022ee100_m *m);
void MTX_RotZ33_(UnkStruct_022ee100_m *m, s32 s, s32 c);
void MTX_Concat33(UnkStruct_022ee100_m *a, UnkStruct_022ee100_m *b, UnkStruct_022ee100_m *c);
void MTX_MultVec33(UnkStruct_022ee100_v *v, UnkStruct_022ee100_m *m, UnkStruct_022ee100_v *d);

void func_ov001_022ee100(UnkStruct_022ee100_v *out, UnkStruct_022ee100_p *p) {
    UnkStruct_022ee100_m m;
    UnkStruct_022ee100_m r;
    UnkStruct_022ee100_s *s = p->unk1DC;
    UnkStruct_022ee100_o *o = p->unk7C;
    UnkStruct_022ee100_v b;
    UnkStruct_022ee100_v a;
    u16 ang;
    s32 w;
    s32 idx;
    s32 i;
    UnkStruct_022ee100_n *n;

    w = s->w;
    a.x = 0;
    a.y = -w / 2;
    a.z = 0;
    b.x = 0;
    b.z = 0;
    b.y = w / 2;
    ang = FX_Atan2(o->pos.y - o->prev.y, o->pos.x - o->prev.x);
    MTX_Identity33_(&m);
    idx = (s32)(ang & 0xffff) >> 4;
    MTX_RotZ33_(&r, SIN_IDX(idx), COS_IDX(idx));
    MTX_Concat33(&m, &r, &m);
    MTX_MultVec33(&a, &m, &a);
    out[1].x = a.x + (o->pos.x + o->ofs.x);
    out[1].y = a.y + (o->pos.y + o->ofs.y);
    out[1].z = o->pos.z + a.z;
    MTX_MultVec33(&b, &m, &b);
    out[0].x = b.x + (o->pos.x + o->ofs.x);
    out[0].y = b.y + (o->pos.y + o->ofs.y);
    out[0].z = o->pos.z + b.z;
    i = 0;
    n = s->list->head;
    if (s->count <= 0)
        return;
    do {
        i++;
        n->x -= 0x6000;
        n->y -= 0x6000;
        n = n->next;
    } while (i < s->count);
}
