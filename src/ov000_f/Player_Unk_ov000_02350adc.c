#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 y;
    s32 z;
} UnkStruct_02350adc_Obj;

typedef struct {
    u8 pad0[2];
    u8 unk2;
    u8 pad3[5 - 3];
    u8 unk5;
    u8 pad6[0x14 - 6];
    s32 y;
    s32 z;
    u8 pad1C[0x30 - 0x1C];
    s16 unk30;
    s16 unk32;
    u8 pad34[0x40 - 0x34];
    s16 unk40;
    s16 unk42;
    u8 pad44[0x58 - 0x44];
    u32 unk58;
    u8 pad5C[0x330 - 0x5C];
    u32 unk330;
    u8 pad334[0x420 - 0x334];
    u16 unk420;
} UnkStruct_02350adc;

s16 ObjSpd_UpSet(s32 v, s32 a, s32 max);
s16 ObjSpd_DownSet(s32 v, s32 a);
u8 ObjAngle_MoveToward8(u8 cur, s32 target, s32 step);

void Player_Unk_ov000_02350adc(UnkStruct_02350adc *w, UnkStruct_02350adc_Obj *o) {
    s32 t;
    u16 m1;
    u16 m2;

    w->unk330 |= 0x1000;
    t = 0x40;
    w->unk58 &= ~0x80;
    w->unk58 |= 0x100;
    if (w->unk330 & 0x100000) {
        if (w->unk58 & 0x10) {
            if (w->unk420 & 0xc0) {
                m1 = t;
                m2 = 0x80;
                if (w->unk5 != 0) {
                    m1 = 0x80;
                    m2 = t;
                }
                if (w->unk420 & m1) {
                    t = -0x40;
                    if (w->unk30 > 0) {
                        w->unk30 = ObjSpd_DownSet(w->unk30, 0x20);
                    }
                }
                if ((w->unk420 & m2) && w->unk30 < 0) {
                    w->unk30 = ObjSpd_DownSet(w->unk30, 0x20);
                }
                w->unk30 = ObjSpd_UpSet(w->unk30, t, 0x200);
            } else {
                w->unk30 = ObjSpd_DownSet(w->unk30, 0x20);
            }
        }
    } else if (!(w->unk58 & 1) && w->unk2 != 0x40 && w->unk2 != 0xc0) {
        if (w->unk5 != 0) {
            if (o->y > w->y) {
                t = -0x40;
            }
        } else {
            if (o->y <= w->y) {
                t = -0x40;
            }
        }
        w->unk30 = ObjSpd_UpSet(w->unk30, t, 0x200);
    }
    if (o->z > 0) {
        w->unk2 = ObjAngle_MoveToward8(w->unk2, 0x40, 2);
        w->unk32 = ObjSpd_UpSet(w->unk32, w->unk40, w->unk42);
        if (w->z < o->z) {
            w->unk58 &= ~1;
            return;
        }
        w->z = o->z;
        w->unk32 = 0;
        w->unk58 |= 1;
        w->unk330 |= 0x100000;
        return;
    }
    w->unk2 = ObjAngle_MoveToward8(w->unk2, 0xc0, 2);
    w->unk32 = ObjSpd_UpSet(w->unk32, -w->unk40, w->unk42);
    if (w->z > o->z) {
        w->unk58 &= ~1;
        return;
    }
    w->z = o->z;
    w->unk32 = 0;
    w->unk30 = 0;
    w->unk58 |= 1;
    w->unk330 |= 0x100000;
}
