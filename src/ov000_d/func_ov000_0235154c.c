#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_0235154c_3E8;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[3];
    u8 unk5;
    u8 pad6[0x10 - 6];
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x28 - 0x24];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x34 - 0x2C];
    s16 unk34;
    u8 pad36[0x3E - 0x36];
    s16 unk3E;
    u8 pad40[0x372 - 0x40];
    s16 unk372;
    u8 pad374[0x3E8 - 0x374];
    UnkStruct_0235154c_3E8 *unk3E8;
    u8 pad3EC[4];
    s32 unk3F0;
    s32 unk3F4;
    u8 pad3F8[4];
    s32 unk3FC;
} UnkStruct_0235154c;

extern s16 data_0206e470[];

s16 ObjSpd_UpSet(s16 v, s32 a, s32 b);

#define ABS_0235154c(x) ((x) < 0 ? -(x) : (x))

void func_ov000_0235154c(UnkStruct_0235154c *w) {
    s32 i;
    s32 r;

    w->unk3F4 -= w->unk34;
    if (w->unk34 > 0) {
        w->unk34 = ObjSpd_UpSet(w->unk34, 0x18, w->unk3E);
        if (w->unk3F4 < w->unk3F0) {
            w->unk3F4 = w->unk3F0;
        }
    } else {
        w->unk34 = ObjSpd_UpSet(w->unk34, -0x18, w->unk3E);
        if (w->unk3F4 > w->unk3F0) {
            w->unk3F4 = w->unk3F0;
        }
    }
    if (w->unk372 < (s16)ABS_0235154c(w->unk34)) {
        w->unk372 = ABS_0235154c(w->unk34);
        if (w->unk372 > w->unk3E) {
            w->unk372 = w->unk3E;
        }
    }
    w->unk1 = (w->unk3F4 & 0xffff) >> 8;
    w->unk1C = w->unk10;
    w->unk20 = w->unk14;
    r = w->unk3FC;
    i = (s32)((u16)((u8)(w->unk1 + w->unk5) << 8) & 0xffff) >> 4;
    w->unk10 = w->unk3E8->unk10 - (s16)((r * data_0206e470[(u32)(i << 1)]) >> 12);
    w->unk14 = w->unk3E8->unk14 + (s16)((r * data_0206e470[(u32)((i << 1) + 1)]) >> 12);
    w->unk28 = w->unk10 - w->unk1C;
    w->unk2A = w->unk14 - w->unk20;
}
