#include <nitro/types.h>

typedef struct UnkStruct_0230f550_v {
    s32 x, y, z;
} UnkStruct_0230f550_v;

typedef struct UnkStruct_0230f550_d {
    s16 unk0;
    u16 unk2;
    UnkStruct_0230f550_v vec;
    u32 unk10;
    u32 *unk14;
    u32 *unk18;
    u32 unk1C[7];
    u8 pad38[0x40 - 0x38];
} UnkStruct_0230f550_d;

typedef struct UnkStruct_0230f550_n {
    s32 unk0;
    UnkStruct_0230f550_v vec;
    u16 unk10;
    u16 unk12;
    u8 pad14[2];
    u16 unk16;
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x38 - 0x20];
    s32 *unk38;
    u8 pad3C[0x5c - 0x3c];
    s16 unk5C;
    u16 unk5E;
    u16 unk60;
    u8 pad62[2];
    u16 unk64;
    u8 pad66[2];
    u16 unk68;
    u16 unk6A;
    u8 pad6C[4];
    s32 unk70;
    u8 pad74[4];
    s32 *unk78;
    u8 unk7C[0xac - 0x7c];
    u32 unkAC;
    u32 unkB0;
    u32 unkB4[7];
    u8 padD0[4];
    struct UnkStruct_0230f550_n *unkD4;
    struct UnkStruct_0230f550_n *unkD8;
} UnkStruct_0230f550_n;

extern UnkStruct_0230f550_d data_ov001_023172cc[];

UnkStruct_0230f550_n *func_ov001_023102fc(void *g);
s32 func_ov001_02310144(UnkStruct_0230f550_n *n);
void func_ov001_0230d28c(void *m, s32 mode, s32 flag, s32 force, u32 ang, s32 y, s32 z);

UnkStruct_0230f550_n *func_ov001_0230f550(void **g, UnkStruct_0230f550_n *parent, s32 type, s32 arg, UnkStruct_0230f550_n *link, u16 ang, s32 y, s32 z) {
    UnkStruct_0230f550_n *n = func_ov001_023102fc(*g);
    UnkStruct_0230f550_d *d = &data_ov001_023172cc[type];
    s32 flag;

    n->unk0 = 2;
    n->unk16 = ang;
    n->unk18 = y;
    n->unk1C = z;
    if (type != 0) {
        if (type == 0x16)
            n->unk18 += 0x800;
    } else {
        n->unk18 += 0x1800;
    }
    n->vec = d->vec;
    n->unk5C = d->unk0;
    n->unk5E = d->unk2;
    n->unk68 = type;
    n->unk6A = arg;
    n->unk70 = -1;
    if (z >= 0x12c000) {
        n->unk78 = parent->unk38;
        n->unk1C -= 0x12c000;
    } else {
        n->unk78 = (s32 *)parent;
    }
    if (d->unk14 != NULL)
        n->unkB0 = *d->unk14;
    else
        n->unkB0 = 0;
    if (d->unk18 != NULL)
        n->unkAC = *d->unk18;
    else
        n->unkAC = 0;
    n->unk60 = 0x10;
    n->unk64 = d->unk10;
    if (n->unk78 != NULL) {
        if (n->unk64 & 0x2000)
            flag = 1;
        else
            flag = 0;
        func_ov001_0230d28c(n->unk7C, *n->unk78, flag, func_ov001_02310144(n), n->unk16, n->unk18, n->unk1C);
    } else {
        n->unk60 |= 0x20;
    }
    n->unkB4[0] = d->unk1C[0];
    n->unkB4[1] = d->unk1C[1];
    n->unkB4[2] = d->unk1C[2];
    n->unkB4[3] = d->unk1C[3];
    n->unkB4[4] = d->unk1C[4];
    n->unkB4[5] = d->unk1C[5];
    n->unkB4[6] = d->unk1C[6];
    n->unkD4 = link;
    if (n->unkD4 != NULL)
        n->unkD8 = link->unkD8;
    else
        n->unkD8 = n;
    switch (type) {
    case 4:
        n->unk12 = 0;
        break;
    case 5:
        n->unk12 = 0x71c;
        break;
    case 6:
        n->unk12 = 0xf8e4;
        break;
    case 0x17:
        n->unk10 = 0xc000;
        break;
    }
    return n;
}
