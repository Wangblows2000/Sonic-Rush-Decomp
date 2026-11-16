#include <nitro/types.h>

typedef struct {
    s32 x, y, z;
} UnkVec_02067670;

typedef struct {
    s32 m[9];
} UnkMtx_02067670;

typedef struct {
    u8 pad0;
    u8 angle;
    u8 pad2[0xe];
    s32 x;
    s32 y;
} UnkObj_02067670;

typedef struct UnkActor_02067670 UnkActor_02067670;

typedef struct {
    UnkObj_02067670 *obj;
    UnkActor_02067670 *unk4;
    UnkActor_02067670 *unk8;
    u16 flags;
    u8 pade[0x12];
    u16 w;
    u16 h;
} UnkEntry_02067670;

struct UnkActor_02067670 {
    u8 pad0[5];
    u8 unk5;
    u8 pad6[0x22];
    s16 unk28;
    s16 unk2a;
    u8 pad2c[0x2c];
    u32 unk58;
    u8 pad5c[0x24];
    UnkObj_02067670 *unk80;
    UnkObj_02067670 *unk84;
};

extern u8 data_022c4ddc;
extern UnkEntry_02067670 *data_022c4e64[];
extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void func_02067c6c(UnkEntry_02067670 *e, s16 *ox, s16 *oy);
void func_02000e98(UnkMtx_02067670 *m, s32 s, s32 c);
void func_02000eb0(UnkVec_02067670 *v, UnkMtx_02067670 *m, UnkVec_02067670 *d);
s32 func_020670bc(UnkEntry_02067670 *e, s32 x, s32 y, s32 p3, s32 dir, u8 *out, s32 p6);

static inline void VecSet_02067670(UnkVec_02067670 *v, s32 x, s32 y, s32 z) {
    v->x = x << 4;
    v->y = y << 4;
    v->z = z << 4;
}

s32 func_02067670(UnkActor_02067670 *a, s32 x, s32 y, s32 p3, s32 dir, u8 *out, s32 p6) {
    UnkEntry_02067670 *e;
    u32 f;
    BOOL nf;
    s32 sx;
    s16 ox;
    s32 best;
    u8 i;
    s32 sy;
    s32 d;
    s32 x8;
    s32 y8;
    s32 idx;
    BOOL flag;
    s16 oy;
    UnkMtx_02067670 m;
    UnkVec_02067670 v;

    sx = x;
    sy = y;
    nf = FALSE;
    flag = FALSE;
    ox = 0;
    oy = 0;
    best = 0x18;
    if (data_022c4ddc == 0)
        return best;
    f = a->unk58 & 0x10;
    if (f == 0)
        nf = TRUE;
    switch ((s32)((a->unk5 + 0x20) & 0xc0) >> 6) {
    case 0:
        if (dir == 2 && (f == 0 || a->unk2a >= 0))
            flag = TRUE;
        break;
    case 1:
        if (dir == 1 && (f == 0 || a->unk28 < 0))
            flag = TRUE;
        break;
    case 2:
        if (dir == 3 && (f == 0 || a->unk2a < 0))
            flag = TRUE;
        break;
    case 3:
        if (dir == 0 && (f == 0 || a->unk28 > 0))
            flag = TRUE;
        break;
    }
    for (i = 0; i < data_022c4ddc; i++) {
        e = data_022c4e64[i];
        if (e->obj == (UnkObj_02067670 *)a || e->obj == NULL || e->w == 0 || e->h == 0)
            continue;
        x = sx;
        y = sy;
        x8 = x << 8;
        y8 = y << 8;
        func_02067c6c(e, &ox, &oy);
        if ((e->flags & 1) && e->obj->angle != 0) {
            VecSet_02067670(&v, x8 - e->obj->x, y8 - e->obj->y, 0);
            idx = (s32)((u16)(-(e->obj->angle << 8)) & 0xffff) >> 4;
            func_02000e98(&m, SIN_IDX(idx), COS_IDX(idx));
            func_02000eb0(&v, &m, &v);
            x = ((v.x >> 4) + e->obj->x) >> 8;
            y = ((v.y >> 4) + e->obj->y) >> 8;
        }
        if (x < (e->obj->x >> 8) - 0x18 + ox || x > ox + ((e->obj->x >> 8) + e->w * 8 + 0x18))
            continue;
        if (y < (e->obj->y >> 8) - 0x18 + oy || y > oy + ((e->obj->y >> 8) + e->h * 8 + 0x18))
            continue;
        d = func_020670bc(e, x, y, p3, dir, out, p6);
        if (best <= d)
            continue;
        best = d;
        if (d <= nf) {
            a->unk84 = e->obj;
            e->unk8 = a;
        }
        if (d <= nf && flag) {
            a->unk80 = e->obj;
            e->unk4 = a;
            if (out != NULL && a->unk5 != 0)
                *out += a->unk5;
        } else if (out != NULL && a->unk5 != 0) {
            *out = a->unk5;
        }
    }
    return best;
}
