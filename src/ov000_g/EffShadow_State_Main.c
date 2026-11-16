#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x10 - 2];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x67 - 0x1C];
    s8 unk67;
} UnkStruct_0231960c_P;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x10 - 2];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x54 - 0x1C];
    u32 unk54;
    u8 pad58[0x80 - 0x58];
    UnkStruct_0231960c_P *unk80;
    u8 pad84[4];
    UnkStruct_0231960c_P *unk88;
} UnkStruct_0231960c;

extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]

void EffShadow_State_Main(UnkStruct_0231960c *w) {
    s32 v;
    s32 bx;
    s32 ax;
    s32 d;
    s16 dx;
    s16 dy;
    s16 s;
    s16 t;
    u8 ang;

    if (w->unk80 == NULL) {
        w->unk54 |= 4;
        return;
    }
    if (w->unk80 == NULL)
        return;
    if (w->unk88 == NULL)
        return;
    w->unk18 = w->unk88->unk18 - 0x800;
    v = -0x1200;
    w->unk10 = w->unk80->unk10;
    w->unk14 = w->unk80->unk14;
    w->unk1 = w->unk80->unk1;
    ax = w->unk80->unk10;
    bx = w->unk88->unk10;
    d = bx - ax;
    ang = w->unk1;
    dx = d >> 8;
    dy = (w->unk88->unk14 + ((w->unk88->unk67 + 2) << 8) - w->unk80->unk14) >> 8;
    if (ang != 0) {
        s = SIN((u16)(ang << 8));
        t = (s * 0xa400) >> 12;
        v = ((t * (d >> 7)) >> 8) - 0x1200;
        if (ang > 0x80)
            v += ((s16)(s << 1) * -((bx - 0x8000 - ax) >> 8)) >> 8;
        else
            v += ((s16)(s << 1) * -((bx + 0x8000 - ax) >> 8)) >> 8;
    }
    if ((v >> 8) > dy)
        dy = v >> 8;
    w->unk10 += dx << 8;
    w->unk14 += dy << 8;
}
