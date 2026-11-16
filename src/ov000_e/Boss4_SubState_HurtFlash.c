#include <nitro/types.h>

typedef struct {
    u8 pad[0x21C];
    u32 unk21C;
    u8 pad220[4];
    s32 unk224;
    u8 pad228[0xBB8 - 0x228];
    s32 unkBB8;
    s32 unkBBC;
    s32 unkBC0;
    s32 unkBC4;
    s32 unkBC8;
    s32 unkBCC;
    u8 padBD0[0xBDC - 0xBD0];
    s32 unkBDC;
} UnkStruct_022ddedc;

void Boss4_Action_StartSpin(UnkStruct_022ddedc *s);
void Boss4_Unk_ov000_022dbc4c(s32 a);
void Boss4_Unk_ov000_022dbb7c(s32 a);
void Boss4_Unk_ov000_022dba50(s32 a);
void Boss4_Unk_ov000_022dbab4(s32 a);
s32 MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
BOOL Boss4_IsTiltPastLimits(UnkStruct_022ddedc *s);

void Boss4_SubState_HurtFlash(UnkStruct_022ddedc *s) {
    s32 d;

    if (s->unk224 > 0x3c) {
        Boss4_Action_StartSpin(s);
        s->unk21C &= ~2;
        Boss4_Unk_ov000_022dbc4c(0);
        Boss4_Unk_ov000_022dbb7c(0);
        Boss4_Unk_ov000_022dba50(0);
        Boss4_Unk_ov000_022dbab4(0);
        return;
    }
    s->unkBB8 += s->unkBC0;
    s->unkBBC += s->unkBC4;
    if (s->unk224 & 2) {
        s->unk21C &= ~2;
        Boss4_Unk_ov000_022dbc4c(0);
        Boss4_Unk_ov000_022dbb7c(0);
        Boss4_Unk_ov000_022dba50(0);
        Boss4_Unk_ov000_022dbab4(0);
    } else {
        s->unk21C |= 2;
        Boss4_Unk_ov000_022dbc4c(1);
        Boss4_Unk_ov000_022dbb7c(1);
        Boss4_Unk_ov000_022dba50(1);
        Boss4_Unk_ov000_022dbab4(1);
    }
    if (!Boss4_IsTiltPastLimits(s))
        return;
    d = s->unkBCC - s->unkBC8;
    if (d < -0x40)
        d = -0x40;
    if (d == 0) {
        s->unkBC8 = s->unkBCC;
        s->unkBC4 = 0;
        return;
    }
    s->unkBC8 += d;
    s->unkBC4 *= -1;
}
