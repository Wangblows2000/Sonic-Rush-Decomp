#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 pad4[0x8 - 4];
    u32 unk8;
    s32 unkC;
    s32 x;
    s32 y;
    u8 pad18[0x1C - 0x18];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x28 - 0x24];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x34 - 0x2C];
    s16 unk34;
    u8 pad36[0x67 - 0x36];
    s8 unk67;
    u8 pad68[0x32C - 0x68];
    u32 unk32C;
    u8 pad330[0x3F0 - 0x330];
    s32 unk3F0;
    s32 unk3F4;
} UnkStruct_02354de4;

extern s16 data_0206e470[];
s32 FX_DivS32(s32 n, s32 d);

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IDX_02354de4(a) ((s32)((a) & 0xffff) >> 4)
#define COS_02354de4(a) data_0206e470[(u32)((IDX_02354de4(a) << 1) + 1)]

void Player_Unk_ov000_02354de4(UnkStruct_02354de4 *w, s32 len, s16 ox, s16 oy, s32 dist, s16 sx, s16 sy) {
    s32 t;
    s32 quarter;
    s32 half;
    s8 q;
    s32 n;
    s16 cs;

    w->unkC += ABS(w->unk34);
    if (ABS(w->unkC) >= dist) {
        if (!(w->unk32C & 1)) {
            w->unk32C |= 1;
            if (w->unk8 & 1) {
                w->unk3F0 += -((sx - ox) << 8);
                if (!(w->unk8 & 2)) {
                    w->unk3F0 -= w->unk67 << 8;
                }
            } else {
                w->unk3F0 += (sx - ox) << 8;
            }
            if (!(w->unk8 & 1)) {
                w->unk3F0 += w->unk67 << 8;
            }
            if (w->unk8 & 2) {
                w->unk3F4 += ((oy >> 2) - sy) << 8;
            } else {
                w->unk3F4 -= ((oy >> 2) - sy) << 8;
            }
        }
        t = w->unkC - dist + (quarter = len >> 2);
        q = FX_DivS32(quarter + ABS(t), half = len >> 1);
        if (q >= (w->unk8 >> 2)) {
            if (!(w->unk32C & 2)) {
                w->unk32C |= 2;
                if (w->unk8 & 1) {
                    w->unk3F0 -= (ox - w->unk67) << 8;
                } else {
                    w->unk3F0 += (ox - w->unk67) << 8;
                }
                if (w->unk8 & 2) {
                    w->unk3F4 -= ((q - 1) * (oy >> 1) + (oy >> 2)) << 8;
                } else {
                    w->unk3F4 += ((q - 1) * (oy >> 1) + (oy >> 2)) << 8;
                }
            }
            n = FX_DivS32((quarter + (t - half * (w->unk8 >> 2))) << 8, dist);
            w->unk2 = ((n << 6) >> 8) + 0xc0;
            if (w->unk8 & 2) {
                sy = -sy;
            }
            if (w->unk8 & 1) {
                sx = -sx;
            }
            if (!((w->unk8 >> 2) & 1)) {
                sx = -sx;
            }
            w->unk1C = w->x;
            w->unk20 = w->y;
            w->x = w->unk3F0 + sx * n;
            w->y = w->unk3F4 + sy * n;
            w->unk28 = w->x - w->unk1C;
            w->unk2A = w->y - w->unk20;
            return;
        }
        q = FX_DivS32(t, len);
        n = FX_DivS32((t - len * q) << 8, len);
        w->unk3 = n + 0xc0;
        w->unk2 = 0xc0;
        if (w->unk8 & 1) {
            w->unk3 = -w->unk3;
        }
        cs = COS_02354de4((u16)(n << 8));
        ox = ox - w->unk67;
        if (w->unk8 & 2) {
            oy = -oy;
        }
        w->unk1C = w->x;
        w->unk20 = w->y;
        if (w->unk8 & 1) {
            w->x = w->unk3F0 - (ox << 8) + ((cs * ox) >> 4);
        } else {
            w->x = w->unk3F0 + (ox << 8) - ((cs * ox) >> 4);
        }
        w->y = w->unk3F4 + ((q * oy) << 8) + oy * n;
        w->unk28 = w->x - w->unk1C;
        w->unk2A = w->y - w->unk20;
        return;
    }
    n = FX_DivS32(w->unkC << 8, dist);
    w->unk2 = 0x100 - ((n << 6) >> 8);
    if ((w->unk8 & 1) && (w->unk8 & 2)) {
        sx = sx - w->unk67;
    }
    if (w->unk8 & 2) {
        sy = -sy;
    }
    if (w->unk8 & 1) {
        sx = -sx;
    }
    w->unk1C = w->x;
    w->unk20 = w->y;
    w->x = w->unk3F0 + sx * n;
    w->y = w->unk3F4 + sy * n;
    w->unk28 = w->x - w->unk1C;
    w->unk2A = w->y - w->unk20;
}
