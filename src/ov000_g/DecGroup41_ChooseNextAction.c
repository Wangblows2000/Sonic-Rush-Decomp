#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x5C - 0x18];
    u16 unk5C;
    u8 pad5E[0x188 - 0x5E];
    s32 unk188;
    s32 unk18C;
} UnkStruct_023218d0;

void DecGroup41_Action_Unk_ov000_02321824(UnkStruct_023218d0 *w);
void DecGroup41_Action_Unk_ov000_02321744(UnkStruct_023218d0 *w, s16 x, s16 y, s32 c);
void DecGroup41_Action_Unk_ov000_023216a4(UnkStruct_023218d0 *w);

void DecGroup41_ChooseNextAction(UnkStruct_023218d0 *w) {
    s16 r;
    s16 b = 6;
    s16 c = 6;
    s16 a = 2;
    s16 d = 2;

    r = Rand() & 0xf;
    if (w->unk14 > (w->unk18C << 8) + 0x2000) {
        a += b;
        b = 0;
    }
    if ((w->unk10 < (w->unk188 << 8) - 0x3000 && (w->unk5C & 1)) ||
        (w->unk10 > (w->unk188 << 8) + 0x3000 && !(w->unk5C & 1))) {
        d += c + a;
        c = 0;
        a = 0;
    }
    if (w->unk14 < (w->unk18C << 8) - 0x2000) {
        b += c + a + d;
        d = 0;
        a = 0;
        c = 0;
    }
    r -= b;
    if (r < 0) {
        DecGroup41_Action_Unk_ov000_02321824(w);
        return;
    }
    r -= c;
    if (r < 0) {
        if (w->unk5C & 1) {
            DecGroup41_Action_Unk_ov000_02321744(w, -0x81 - (Rand() & 0xfe), 0xff - (Rand() & 0x1fe), Rand() & 7);
        } else {
            DecGroup41_Action_Unk_ov000_02321744(w, 0x17f - (Rand() & 0xfe), 0xff - (Rand() & 0x1fe), Rand() & 7);
        }
        return;
    }
    r -= a;
    if (r < 0) {
        if (w->unk5C & 1) {
            DecGroup41_Action_Unk_ov000_02321744(w, -0x81 - (Rand() & 0x1fe), -0xc1 - (Rand() & 0x7e), Rand() & 7);
        } else {
            DecGroup41_Action_Unk_ov000_02321744(w, 0x27f - (Rand() & 0x1fe), -0xc1 - (Rand() & 0x7e), Rand() & 7);
        }
        return;
    }
    r -= d;
    if (r < 0) {
        DecGroup41_Action_Unk_ov000_023216a4(w);
    }
}
