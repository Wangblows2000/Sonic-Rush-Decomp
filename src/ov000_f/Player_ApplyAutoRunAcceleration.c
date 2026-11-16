#include <nitro/types.h>

typedef struct UnkStruct_02347680 UnkStruct_02347680;

struct UnkStruct_02347680 {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x34 - 2];
    s16 unk34;
    u8 pad36[0x5C - 0x36];
    u16 unk5C;
    u8 pad5E[0x324 - 0x5E];
    u8 unk324;
    u8 pad325[0x32C - 0x325];
    u32 unk32C;
    u8 pad330[0x352 - 0x330];
    s16 unk352;
    u8 pad354[0x36E - 0x354];
    s16 unk36E;
    u8 pad370[0x372 - 0x370];
    s16 unk372;
    u8 pad374[0x37A - 0x374];
    s16 unk37A;
};

typedef struct {
    s16 v;
    u8 pad2[0x30 - 2];
} UnkPhys_02347680;

extern s16 data_0206e470[];
extern UnkPhys_02347680 gPlayerPhysicsTable[];
extern UnkPhys_02347680 data_ov000_02368b7e[];
extern UnkPhys_02347680 data_ov000_02368b80[];
extern UnkPhys_02347680 data_ov000_02368b8a[];
extern UnkPhys_02347680 data_ov000_02368b8c[];
extern UnkPhys_02347680 data_ov000_02368b8e[];
extern UnkPhys_02347680 data_ov000_02368b90[];

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IDX_02347680(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_02347680(a) data_0206e470[(u32)(IDX_02347680(a) << 1)]

void Player_UpdateHighSpeedTimer(UnkStruct_02347680 *w);
s16 ObjSpd_DownSet(s16 v, s32 dec);
s16 ObjSpd_UpSet(s16 v, s32 acc, s32 top);

void Player_ApplyAutoRunAcceleration(UnkStruct_02347680 *w) {
    s32 acc;
    s16 top;
    s32 dec;
    s16 s;
    s16 t;

    w->unk372 = 0;
    acc = gPlayerPhysicsTable[w->unk324].v;
    dec = data_ov000_02368b80[w->unk324].v;
    top = data_ov000_02368b7e[w->unk324].v;
    if (w->unk32C & 0x100) {
        top = data_ov000_02368b8a[w->unk324].v;
    }
    if (w->unk32C & 0x80) {
        acc = data_ov000_02368b8c[w->unk324].v;
        top = data_ov000_02368b8e[w->unk324].v;
        dec = data_ov000_02368b90[w->unk324].v;
    }
    Player_UpdateHighSpeedTimer(w);
    if (w->unk1 != 0) {
        s = (w->unk36E * SIN_02347680((u16)(w->unk1 << 8))) >> 12;
        if (s > 0) {
            top += s;
        }
    }
    if (w->unk352 != 0) {
        dec = 0;
    } else {
        t = 0;
        if (ABS(w->unk34) > w->unk37A) {
            t = ((ABS(w->unk34) - w->unk37A) << 8) / (top - w->unk37A);
            if (t > 0x100) {
                t = 0x100;
            }
            t = (t * 0xf8) >> 8;
        }
        acc = (s16)(acc - ((acc * t) >> 8));
    }
    if (w->unk32C & 0x4000000) {
        acc >>= 1;
        dec >>= 1;
    }
    if (!(w->unk5C & 1)) {
        if (w->unk34 < 0) {
            w->unk34 = ObjSpd_DownSet(w->unk34, dec);
        }
        w->unk34 = ObjSpd_UpSet(w->unk34, acc, top);
    } else {
        if (w->unk34 > 0) {
            w->unk34 = ObjSpd_DownSet(w->unk34, dec);
        }
        w->unk34 = ObjSpd_UpSet(w->unk34, -acc, top);
    }
}
