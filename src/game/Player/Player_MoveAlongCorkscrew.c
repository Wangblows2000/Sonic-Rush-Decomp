#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
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
    u8 pad68[0x3F0 - 0x68];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
} UnkStruct_02355254;

extern s16 data_0206e470[];

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IDX_02355254(a) ((s32)((a) & 0xffff) >> 4)
#define COS_02355254(a) data_0206e470[(u32)((IDX_02355254(a) << 1) + 1)]

void Player_MoveAlongCorkscrew(UnkStruct_02355254 *w, s32 len, s32 rx, s16 ry) {
    s32 t;
    s8 q;
    s32 n;
    s16 cs;

    w->unkC += ABS(w->unk34);
    t = w->unkC;
    q = t / len;
    n = ((t - len * q) << 8) / len;
    w->unk2 = n;
    if (w->unk8 & 1) {
        w->unk2 = -w->unk2;
    }
    if (w->unk2 < 0x40) {
        w->unk1 = w->unk2;
    } else if (w->unk2 < 0x80) {
        w->unk1 = 0x80 - w->unk2;
    } else if (w->unk2 < 0xc0) {
        w->unk1 = w->unk2 - 0x80;
    } else {
        w->unk1 = 0x100 - w->unk2;
    }
    w->unk1 = w->unk1 >> 1;
    if (w->unk2 >= 0x80) {
        w->unk1 = -w->unk1;
    }
    w->unk1 = -w->unk1;
    if (w->unk3F8 == 2) {
        w->unk2 = -w->unk2;
    }
    cs = COS_02355254((u16)(n << 8));
    ry = ry - w->unk67;
    if (w->unk8 & 2) {
        ry = -ry;
    }
    w->unk1C = w->x;
    w->unk20 = w->y;
    if (w->unk8 & 1) {
        w->x = w->unk3F0 - ((q * rx) << 8) - rx * n;
    } else {
        w->x = w->unk3F0 + ((q * rx) << 8) + rx * n;
    }
    w->y = w->unk3F4 + (ry << 8) - ((cs * ry) >> 4);
    w->unk28 = w->x - w->unk1C;
    w->unk2A = w->y - w->unk20;
}
