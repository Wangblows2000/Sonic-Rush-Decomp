#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
    u8 pad4[4];
    u32 unk8;
    s32 unkC;
    u8 pad10[0x3E8 - 0x10];
    void *unk3E8;
} UnkStruct_02314e60_P;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02314e60_V;

static inline void SetVec(UnkStruct_02314e60_V *v, s32 x, s32 y, s32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

typedef struct {
    u8 pad0[0x18];
    UnkStruct_02314e60_V unk18;
    s32 mtx[9];
    u8 pad48[0xA0 - 0x48];
    void *unkA0;
} UnkStruct_02314e60_M;

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u8 pad58[0x5C - 0x58];
    u16 unk5C;
    u8 pad5E[0x128 - 0x5E];
    UnkStruct_02314e60_M *unk128;
    u8 pad12C[0x1F4 - 0x12C];
    u32 unk1F4;
    u8 pad1F8[0x204 - 0x1F8];
    UnkStruct_02314e60_P *unk204;
} UnkStruct_02314e60;

extern s16 data_0206e470[];
extern s16 data_02072070[];
extern s16 data_0206f470[];

void func_020319a4(void *p, void *a, s32 b, s32 c, s32 d);
void MTX_Identity33_(void *m);
void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);

void func_ov000_02314e60(UnkStruct_02314e60 *w) {
    UnkStruct_02314e60_P *p = w->unk204;
    UnkStruct_02314e60_M *m;
    s32 idx;

    if (p != NULL) {
        u8 mtx[0x24];
        if (p->unk3E8 != w) {
            w->unk5C |= 0x200;
            w->unk54 &= ~2;
            w->unk204 = NULL;
            func_020319a4(w->unk128, w->unk128->unkA0, 1, 0, 0);
            return;
        }
        m = w->unk128;
        MTX_Identity33_(m->mtx);
        idx = (s32)((u16)p->unk8 & 0xffff) >> 4;
        MTX_RotX33_(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
        MTX_Concat33(m->mtx, mtx, m->mtx);
        idx = (s32)((u16)(-((u8)(p->unk3 - 0x40) << 8)) & 0xffff) >> 4;
        MTX_RotY33_(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
        MTX_Concat33(m->mtx, mtx, m->mtx);
        SetVec(&m->unk18, 0x34cc, 0x34cc, ((p->unkC << 8) / 0xa000 * 0x34cc) >> 8);
        if (p->unk3 < 0x80)
            w->unk1F4 |= 1;
        else
            w->unk1F4 &= ~1;
    } else {
        u8 mtx2[0x24];
        m = w->unk128;
        w->unk1F4 &= ~1;
        MTX_Identity33_(m->mtx);
        MTX_RotX33_(mtx2, data_02072070[0], data_02072070[1]);
        MTX_Concat33(m->mtx, mtx2, m->mtx);
        MTX_RotY33_(mtx2, data_0206f470[0], data_0206f470[1]);
        MTX_Concat33(m->mtx, mtx2, m->mtx);
        SetVec(&w->unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    }
}
