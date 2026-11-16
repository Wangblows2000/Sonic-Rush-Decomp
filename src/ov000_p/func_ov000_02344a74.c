#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u32 unk4;
} UnkStruct_02344a74_Spr;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2;
    u8 unk3;
    u8 pad4[0x10 - 4];
    s32 x;
    s32 y;
    s32 z;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x54 - 0x24];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x124 - 0x5E];
    UnkStruct_02344a74_Spr *unk124;
    u8 pad128[0x325 - 0x128];
    u8 unk325;
    u8 pad326[0x328 - 0x326];
    s16 unk328;
} UnkStruct_02344a74;

typedef struct {
    s8 dx;
    s8 dy;
    s8 z : 4;
    u8 ang : 4;
    u8 f3;
} UnkStruct_02344a74_Rec;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 pad1C[0x20 - 0x1C];
    u32 unk20;
} UnkStruct_02344a74_4560;

typedef struct {
    u8 pad0[0x50];
    UnkStruct_02344a74_Rec *unk50;
} UnkStruct_02344a74_4590;

typedef struct {
    u32 flags;
    s32 x;
    s32 y;
} UnkStruct_02344a74_Cam;

extern UnkStruct_02344a74_4560 data_022c4560;
extern UnkStruct_02344a74_4590 data_022c4590;
extern UnkStruct_02344a74_Cam data_ov000_0236aad8;

void func_ov000_0234c4a4(UnkStruct_02344a74 *w, s16 state);
void func_ov000_023470a0(UnkStruct_02344a74 *w, s32 v);

void func_ov000_02344a74(UnkStruct_02344a74 *w) {
    UnkStruct_02344a74_Rec *rec;
    u32 n;
    s32 i;
    s32 t;
    s32 dx;
    s32 dy;
    s16 st;

    if (!(data_022c4560.unk18 & 0x80)) {
        return;
    }
    if (w->unk325 == 0) {
        return;
    }
    if (data_ov000_0236aad8.x - 0x2000 < w->x && data_ov000_0236aad8.x + 0x12000 > w->x &&
        data_ov000_0236aad8.y - 0x2000 < w->y && data_ov000_0236aad8.y + 0x10000 > w->y) {
        w->unk124->unk4 &= ~1;
    } else {
        w->unk124->unk4 |= 1;
    }
    n = data_022c4560.unk20 >> 2;
    if (n >= 0xc4e || !(data_022c4560.unk18 & 1)) {
        w->unk58 |= 0x2100;
        w->unk54 |= 2;
        w->unk5C |= 0x20;
        return;
    }
    i = n - 1;
    rec = data_022c4590.unk50;
    if (!(data_022c4560.unk20 & 1)) {
        if (i >= 0 && rec[i].f3 == 0xff) {
            data_022c4560.unk18 &= ~0x80;
            w->unk58 |= 0x2100;
            w->unk54 |= 2;
            w->unk5C |= 0x20;
            return;
        }
        w->unk5C &= ~0x20;
        if (!(data_022c4560.unk20 & 2)) {
            if (i >= 0) {
                w->unk1C = w->x;
                w->unk20 = w->y;
                w->x += (rec[i].dx >> 2) << 8;
                w->y += (rec[i].dy >> 2) << 8;
                w->z += (rec[i].z >> 2) << 8;
                {
                s32 m;
                t = rec[i + 1].dx;
                m = (t >> 2) * 3;
                w->x += (t - m) << 8;
                t = rec[i + 1].dy;
                m = (t >> 2) * 3;
                w->y += (t - m) << 8;
                t = rec[i + 1].z;
                m = (t >> 2) * 3;
                w->z += (t - m) << 8;
                }
            }
        } else if (i >= -1) {
            if (i == -1) {
                w->x += (rec[i + 1].dx >> 1) << 8;
                w->y += (rec[i + 1].dy >> 1) << 8;
                w->z += (rec[i + 1].z >> 1) << 8;
            } else {
                w->x += (rec[i + 1].dx >> 2) << 8;
                w->y += (rec[i + 1].dy >> 2) << 8;
                w->z += (rec[i + 1].z >> 2) << 8;
            }
            w->x += (rec[i + 1].dx >> 2) << 8;
            w->y += (rec[i + 1].dy >> 2) << 8;
            w->z += (rec[i + 1].z >> 2) << 8;
        }
    } else {
        w->unk5C |= 0x20;
    }
    if (i < 0) {
        i = 0;
    }
    w->unk1 = rec[i].ang << 4;
    if (w->unk328 != (rec[i].f3 & 0x7f)) {
        w->unk5C &= ~0x400;
        func_ov000_0234c4a4(w, (rec[i].f3 & 0x7f));
        if (w->unk328 >= 0x3d && w->unk328 <= 0x42) {
            dy = w->unk20 - w->y;
            if (dy < 0) {
                dy = -dy;
            }
            dx = w->unk1C - w->x;
            if (dx < 0) {
                dx = -dx;
            }
            func_ov000_023470a0(w, ((dx + dy) << 14) >> 16);
        }
        st = w->unk328;
        if ((st < 0x38 || st > 0x3b) && st != 4 && (st < 0xf || st > 0x10)) {
            w->unk5C |= 4;
        }
        if ((rec[i].f3 & 0x80)) {
            w->unk5C |= 1;
        } else {
            w->unk5C &= ~1;
        }
    }
    if (w->unk328 == 0x2f) {
        w->unk3 = 0xc0;
    } else {
        w->unk3 = 0;
    }
    st = w->unk328;
    if ((st >= 1 && st <= 2) || (st >= 0xf && st <= 0x19) || (st >= 0x34 && st <= 0x3b)) {
        w->unk58 &= ~1;
        w->unk1 = 0;
    } else {
        w->unk58 |= 1;
    }
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
}
