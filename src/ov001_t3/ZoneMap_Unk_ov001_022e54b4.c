#include <nitro/types.h>

typedef struct {
    s32 m[4];
} UnkMtx22_022e54b4;

typedef struct {
    s32 x;
    s32 y;
    s32 unk8;
    s32 unkc;
    u32 unk10;
    u8 pad14[0x30 - 0x14];
    s32 unk30;
    u16 unk34;
    u16 unk36;
    u8 pad38[0x50 - 0x38];
    s32 unk50;
    s32 idx;
    s32 unk58;
    u8 pad5c[4];
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6c;
} UnkStruct_022e54b4;

typedef struct {
    u16 v[16];
} UnkEntry_022e54b4;

extern UnkEntry_022e54b4 data_ov001_0231639c[];
extern u8 data_02089264[];

void MTX_Identity22_(UnkMtx22_022e54b4 *m);
void ZoneMap_EnableBlend(s32 a);
void ZoneMap_Unk_ov001_022e519c(s32 x, s32 y, u16 a, u16 b, u16 c, u16 d, u16 e, u16 f, u16 g, u16 h, u16 i, s32 j);
void G2x_SetBGyAffine_(void *dst, UnkMtx22_022e54b4 *m, s32 a, s32 b, s32 c, s32 d);
void ZoneMap_Unk_ov001_022e2bd8(u16 a, u16 b, u16 c, u16 d, u16 e);
void ScreenTransfer_QueueRectRaw(s32 a, u16 b, s32 c, u16 d, u32 e, s32 f, u16 g, u16 h, s32 i, s32 j, s32 k, u16 l);

#pragma thumb on
void ZoneMap_Unk_ov001_022e54b4(UnkStruct_022e54b4 *p) {
    UnkEntry_022e54b4 *t;
    u16 cur;
    UnkMtx22_022e54b4 m;

    MTX_Identity22_(&m);
    ZoneMap_EnableBlend(1);
    if (p->idx > 0) {
        t = &data_ov001_0231639c[p->idx - 1];
        if (p->unk64 != 0) {
            ZoneMap_Unk_ov001_022e519c(p->x, p->y, t->v[0], t->v[1], t->v[2], t->v[3], t->v[4], t->v[5], t->v[6], t->v[7], t->v[8], p->unk60);
        } else {
            G2x_SetBGyAffine_(data_02089264, &m, 0, 0, p->x % 8 + 8, p->y);
        }
        if (p->unk58 != 0) {
            ZoneMap_Unk_ov001_022e2bd8(t->v[9], t->v[10], t->v[12], t->v[13], t->v[15]);
            ZoneMap_Unk_ov001_022e2bd8(t->v[10], t->v[11], t->v[13], t->v[14], t->v[15]);
        }
    } else if (p->unk64 != 0) {
        ZoneMap_Unk_ov001_022e519c(p->x, p->y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
        G2x_SetBGyAffine_(data_02089264, &m, 0, 0, p->x % 8 + 8, p->y);
    }
    cur = p->x / 8;
    if (cur != p->unk6c / 8 || p->unk68 == 1) {
        ScreenTransfer_QueueRectRaw(p->unk50, cur - 1, 0, p->unk8 / 8, p->unk10 & 0x1000, p->unk30, p->unk34, p->unk36, 0, 0, 0x23, p->unkc / 8);
        p->unk68 = 0;
    }
    p->unk6c = p->x;
}
#pragma thumb reset
