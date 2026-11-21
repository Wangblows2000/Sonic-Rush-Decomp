#include <nitro/types.h>

typedef struct UnkStruct_022de774_v {
    s32 x, y, z;
} UnkStruct_022de774_v;

typedef struct UnkStruct_022de774_s {
    s32 m[9];
    UnkStruct_022de774_v pos;
    u8 pad[0x138 - 0x30];
} UnkStruct_022de774_s;

typedef struct UnkStruct_022de774 {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x18];
    UnkStruct_022de774_v vec[2];
    UnkStruct_022de774_v vel[2];
    u16 ang[2][2];
    u16 rot[2];
    u8 pad5C[0x24];
    UnkStruct_022de774_s sub[2];
} UnkStruct_022de774;

typedef struct UnkStruct_022de774_p {
    u8 pad0[4];
    u32 unk4;
} UnkStruct_022de774_p;

typedef struct UnkStruct_022de774_m {
    s32 m[9];
} UnkStruct_022de774_m;

extern volatile u16 gPad;
extern s16 data_0206e470[];

void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void TransformDemo_Unk_ov001_022de774(UnkStruct_022de774_p *p, UnkStruct_022de774 *w) {
    UnkStruct_022de774_m m1;
    UnkStruct_022de774_m m2;
    UnkStruct_022de774_m m3;
    UnkStruct_022de774_m m4;
    u8 i;
    u16 keys;

    if (p->unk4 & 0x40000000) {
        for (i = 0; i < 2; i++) {
            keys = gPad;
            if (keys & 0x40)
                w->vec[i].y += 0x1800;
            else if (keys & 0x80)
                w->vec[i].y -= 0x1800;
            if (gPad & 0x200)
                w->ang[i][1] -= 0x100;
            if (gPad & 0x100)
                w->ang[i][1] += 0x100;
            MTX_RotX33_(&m1, data_0206e470[((w->ang[i][0] & 0xffff) >> 4) * 2], data_0206e470[((w->ang[i][0] & 0xffff) >> 4) * 2 + 1]);
            MTX_RotY33_(&m2, data_0206e470[((w->ang[i][1] & 0xffff) >> 4) * 2], data_0206e470[((w->ang[i][1] & 0xffff) >> 4) * 2 + 1]);
            MTX_Concat33(&m1, &m2, &w->sub[i]);
            if (gPad & 4)
                w->rot[i] -= 0x200;
            if (gPad & 8)
                w->rot[i] += 0x200;
            w->vec[i].z = FX_Mul(data_0206e470[(w->rot[i] >> 4) * 2 + 1], 0x4000);
            w->vec[i].x = FX_Mul(data_0206e470[(w->rot[i] >> 4) * 2], 0x4000);
            w->sub[i].pos.x = w->vec[i].x;
            w->sub[i].pos.y = w->vec[i].y;
            w->sub[i].pos.z = w->vec[i].z;
        }
        w->unk4 = w->sub[0].pos.y + 0x5000;
        return;
    } else {
        for (i = 0; i < 2; i++) {
            if (w->vec[i].y < 0)
                w->vel[i].y -= 0x10;
            else
                w->vel[i].y += 0x10;
            w->vec[i].y += w->vel[i].y;
            MTX_RotX33_(&m3, data_0206e470[((w->ang[i][0] & 0xffff) >> 4) * 2], data_0206e470[((w->ang[i][0] & 0xffff) >> 4) * 2 + 1]);
            MTX_RotY33_(&m4, data_0206e470[((w->ang[i][1] & 0xffff) >> 4) * 2], data_0206e470[((w->ang[i][1] & 0xffff) >> 4) * 2 + 1]);
            MTX_Concat33(&m3, &m4, &w->sub[i]);
            w->rot[i] += 0x100;
            w->vec[i].z = FX_Mul(data_0206e470[(w->rot[i] >> 4) * 2 + 1], 0x4000);
            w->vec[i].x = FX_Mul(data_0206e470[(w->rot[i] >> 4) * 2], 0x4000);
            w->sub[i].pos.x = w->vec[i].x;
            w->sub[i].pos.y = w->vec[i].y;
            w->sub[i].pos.z = w->vec[i].z;
        }
    }
    w->unk4 = w->sub[0].pos.y + 0x5000;
}
