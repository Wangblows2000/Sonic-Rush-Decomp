#include <nitro/types.h>

typedef struct UnkStruct_0230fe50 UnkStruct_0230fe50;

typedef struct {
    u8 pad[0x10];
    s32 unk10;
    u8 pad1[0x80 - 0x14];
    UnkStruct_0230fe50 *unk80;
} UnkStruct_0230fe50_o;

struct UnkStruct_0230fe50 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 pad4[3];
    s8 unk7;
    u32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x30 - 0x18];
    s16 unk30;
    u8 pad32[0x1bc - 0x32];
    UnkStruct_0230fe50_o *unk1BC;
    u8 pad1C0[0x1ec - 0x1c0];
    s32 unk1EC;
};

u8 ObjAngle_MoveToward8(u8 a, u8 b, s32 c);
void Player_ApplySlopeForce(UnkStruct_0230fe50_o *o, UnkStruct_0230fe50 *s);

void LandUnrest_State_Unk_ov000_0230fe50(UnkStruct_0230fe50 *s) {
    u8 a;
    s8 b;

    s->unk2 = s->unk1;
    s->unk30 = 0;
    if (s->unk1BC != NULL) {
        if (s->unk1BC->unk80 == s) {
            s->unk30 = 0x100;
            s->unk3 = (s->unk1BC->unk10 - s->unk10) >> 8;
            s->unk1 = ObjAngle_MoveToward8(s->unk1, s->unk3, 2);
            Player_ApplySlopeForce(s->unk1BC, s);
            if (s->unk1 != 0) {
                if (s->unk1 > 0x80)
                    s->unk8 = (0x100 - s->unk1) >> 1;
                else
                    s->unk8 = 0x100 - (s->unk1 >> 1);
                if (s->unk8 < 4 || s->unk8 > 0xfc)
                    s->unk8 = 0;
            } else {
                s->unk8 = 0;
            }
        }
    } else {
        a = s->unk1;
        if (a != 0) {
            if (s->unk8 == 0) {
                if (a < 0x80) {
                    s->unk1 = a - 2;
                    if (s->unk1 > 0x80)
                        s->unk1 = 0;
                }
                if (s->unk1 > 0x80) {
                    s->unk1 += 2;
                    if (s->unk1 < 0x80)
                        s->unk1 = 0;
                }
            } else {
                if (s->unk8 > 0x80) {
                    s->unk1 = a - 2;
                    if (s->unk8 < s->unk1)
                        s->unk1 = s->unk8;
                }
                if (s->unk8 < 0x80) {
                    s->unk1 += 2;
                    if (s->unk8 > s->unk1)
                        s->unk1 = s->unk8;
                }
            }
            if (s->unk8 == s->unk1) {
                if (s->unk8 < 4 || s->unk8 > 0xfc) {
                    s->unk8 = 0;
                    s->unk1 = 0;
                } else if (s->unk1 > 0x80) {
                    s->unk8 = (0x100 - s->unk1) >> 1;
                } else {
                    s->unk8 = 0x100 - (s->unk1 >> 1);
                }
            }
        }
        if ((s->unk14 >> 8) != s->unk1EC) {
            s->unk30 = -0x100;
            s->unkC = 0;
        }
    }
    if (s->unk30 > 0) {
        if (s->unkC >= 8)
            s->unk30 = 0;
        else
            s->unkC++;
    } else if (s->unk30 < 0) {
        if ((s->unk14 >> 8) <= s->unk1EC) {
            s->unk30 = 0;
            s->unkC = 0;
            s->unk14 = s->unk1EC << 8;
        }
    }
    if (s->unk1 == 0)
        return;
    if (s->unk2 < 0x80)
        b = -(s->unk2 >> 1);
    else
        b = -((0x100 - s->unk2) >> 1);
    if (s->unk1 < 0x80)
        s->unk7 = -(s->unk1 >> 1);
    else
        s->unk7 = -((0x100 - s->unk1) >> 1);
    if (b != s->unk7)
        s->unk30 += (b - s->unk7) << 8;
    if ((s->unk14 >> 8) < s->unk1EC)
        s->unk30 = 0;
}
