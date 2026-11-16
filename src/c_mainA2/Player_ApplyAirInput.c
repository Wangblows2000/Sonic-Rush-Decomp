#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x2E - 2];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    u8 pad36[0x5C - 0x36];
    u16 unk5C;
    u8 pad5E[0x328 - 0x5E];
    s16 unk328;
    u8 pad32A[0x32C - 0x32A];
    u32 unk32C;
    u8 pad330[0x352 - 0x330];
    s16 unk352;
    u8 pad354[0x356 - 0x354];
    s16 unk356;
    s16 unk358;
    s16 unk35A;
    u8 pad35C[0x364 - 0x35C];
    s16 unk364;
    s16 unk366;
    s16 unk368;
    s16 unk36A;
    u8 pad36C[0x36E - 0x36C];
    s16 unk36E;
    u8 pad370[0x372 - 0x370];
    s16 unk372;
    u8 pad374[0x37A - 0x374];
    s16 unk37A;
    u8 pad37C[0x3AC - 0x37C];
    s16 unk3AC;
    u8 pad3AE[0x3C4 - 0x3AE];
    s16 unk3C4;
    u8 pad3C6[0x420 - 0x3C6];
    u16 unk420;
} UnkStruct_0234739c;

extern s16 data_0206e470[];

#define SIN_0234739c(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define ABS_0234739c(x) ((x) < 0 ? -(x) : (x))
#define CLAMP_0234739c(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

void Player_UpdateHighSpeedTimer(UnkStruct_0234739c *w);
s16 ObjSpd_DownSet(s32 v, s32 a);
s16 ObjSpd_UpSet(s32 v, s32 a, s32 max);
void Player_ApplyVerticalInput(UnkStruct_0234739c *w);

void Player_ApplyAirInput(UnkStruct_0234739c *w) {
    s32 airAcc;
    s32 dec0 = w->unk35A;
    s32 acc = w->unk356;
    s16 max = w->unk358;
    s32 dec = w->unk35A;
    s32 k;
    s32 t;
    s32 nmax;

    w->unk372 = 0;
    if (w->unk32C & 0x100) {
        max = w->unk364;
    }
    if (w->unk32C & 0x80) {
        acc = w->unk366;
        dec = w->unk36A;
        max = w->unk368;
    }
    Player_UpdateHighSpeedTimer(w);
    if (((w->unk1 + 0x20) & 0xc0) || w->unk1 == 0xe0) {
        dec = (dec << 14) >> 16;
    }
    if (w->unk352 != 0) {
        airAcc = (acc << 14) >> 16;
        dec = 0;
    } else {
        k = 0;
        if (ABS_0234739c(w->unk2E) > w->unk37A) {
            k = (s16)(((ABS_0234739c(w->unk2E) - w->unk37A) << 8) / (max - w->unk37A));
            if (k > 0x100) {
                k = 0x100;
            }
            k = ((s16)k * 0xf8 << 8) >> 16;
        }
        airAcc = (s16)(acc - ((acc * k) >> 8));
    }
    if (w->unk32C & 0x4000000) {
        airAcc >>= 1;
        dec >>= 1;
    }
    if (w->unk3C4 != 0) {
        return;
    }
    if (w->unk420 & 0x30) {
        if (w->unk420 & 0x10) {
            w->unk5C &= ~1;
            if (w->unk2E < 0) {
                w->unk2E = ObjSpd_DownSet(w->unk2E, (s16)(dec << 1));
            }
            if (w->unk34 < 0) {
                w->unk34 = ObjSpd_DownSet(w->unk34, (s16)(dec0 << 1));
            }
            w->unk2E = ObjSpd_UpSet(w->unk2E, airAcc, max);
        } else {
            w->unk5C |= 1;
            if (w->unk2E > 0) {
                w->unk2E = ObjSpd_DownSet(w->unk2E, (s16)(dec << 1));
            }
            if (w->unk34 > 0) {
                w->unk34 = ObjSpd_DownSet(w->unk34, (s16)(dec0 << 1));
            }
            w->unk2E = ObjSpd_UpSet(w->unk2E, -airAcc, max);
        }
    } else {
        t = w->unk2E;
        nmax = -max;
        if (t < nmax) {
            t = nmax;
        } else if (t > max) {
            t = max;
        }
        w->unk2E = t;
        w->unk34 = CLAMP_0234739c(w->unk34, nmax, max);
        w->unk3AC = 0;
        w->unk2E = ObjSpd_DownSet(w->unk2E, dec);
        w->unk34 = ObjSpd_DownSet(w->unk34, dec0);
    }
    Player_ApplyVerticalInput(w);
}
