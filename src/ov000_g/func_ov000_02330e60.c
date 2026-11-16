#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x5C - 0x1C];
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
} UnkStruct_02330e60_P;

typedef struct {
    u8 pad0[0x24];
    s32 mtx[9];
} UnkStruct_02330e60_M;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x54 - 0x1C];
    u32 unk54;
    u8 pad58[0x5C - 0x58];
    u16 unk5C;
    u8 pad5E[0x88 - 0x5E];
    UnkStruct_02330e60_P *unk88;
    u8 pad8C[0x128 - 0x8C];
    UnkStruct_02330e60_M *unk128;
    u8 pad12C[0x344 - 0x12C];
    UnkStruct_02330e60_M unk344;
    u8 pad[0x49C - 0x344 - sizeof(UnkStruct_02330e60_M)];
    u32 unk49C;
    u8 pad4A0[0x764 - 0x4A0];
    u16 unk764;
    u16 unk766;
} UnkStruct_02330e60;

extern s16 data_0206e470[];
extern s16 data_02072070[];

void MTX_Identity33_(void *m);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_RotZ33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);

void func_ov000_02330e60(UnkStruct_02330e60 *w) {
    u8 mtx[0x24];
    UnkStruct_02330e60_P *p = w->unk88;
    UnkStruct_02330e60_M *m = w->unk128;
    s32 idx;
    s16 c;
    s16 s;

    if (p != NULL) {
        w->unk10 = p->unk10;
        w->unk14 = w->unk88->unk14;
        w->unk18 = w->unk88->unk18;
        w->unk5C &= ~0x200;
        w->unk5C |= w->unk88->unk5C & 0x200;
        if (w->unkC & 4)
            w->unk49C |= 0x80;
        else
            w->unk49C &= ~0x80;
        MTX_Identity33_(m->mtx);
        idx = (s32)(w->unk764 & 0xffff) >> 4;
        MTX_RotY33_(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
        MTX_Concat33(m->mtx, mtx, m->mtx);
        c = data_02072070[1];
        s = data_02072070[0];
        MTX_RotZ33_(mtx, s, c);
        MTX_Concat33(m->mtx, mtx, m->mtx);
        m = &w->unk344;
        MTX_Identity33_(m->mtx);
        idx = (s32)((u16)(-w->unk766) & 0xffff) >> 4;
        MTX_RotY33_(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
        MTX_Concat33(m->mtx, mtx, m->mtx);
        MTX_RotZ33_(mtx, s, c);
        MTX_Concat33(m->mtx, mtx, m->mtx);
        if (!(p->unk32C & 0x10000000) || (p->unk32C & 0x20000000))
            w->unk54 |= 4;
    }
    w->unk764 += 0x600;
    w->unk766 += 0x600;
    w->unk764 &= 0x7fff;
    w->unk766 &= 0x7fff;
    w->unkC++;
}
