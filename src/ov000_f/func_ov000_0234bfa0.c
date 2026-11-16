#include <nitro/types.h>

typedef struct UnkStruct_0234bfa0 UnkStruct_0234bfa0;

struct UnkStruct_0234bfa0 {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xC - 2];
    s32 unkC;
    s32 x;
    u8 pad14[0x32 - 0x14];
    s16 unk32;
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x2A0 - 0x5E];
    u8 unk2A0[0x324 - 0x2A0];
    u8 unk324;
    u8 unk325;
    u8 pad326[0x328 - 0x326];
    s16 unk328;
    u8 pad32A[0x330 - 0x32A];
    u32 unk330;
    u8 pad334[0x338 - 0x334];
    void (*unk338)(UnkStruct_0234bfa0 *w);
    void (*unk33C)(UnkStruct_0234bfa0 *w);
    void (*unk340)(UnkStruct_0234bfa0 *w);
    u8 pad344[0x420 - 0x344];
    u16 unk420;
    u16 unk422;
};

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[0x24 - 0x20];
    s32 unk24;
} UnkStruct_0234bfa0_4534;

extern UnkStruct_0234bfa0_4534 data_022c4534;

void Player_SetAnimation(UnkStruct_0234bfa0 *w, s32 state);
BOOL func_ov000_0234be10(UnkStruct_0234bfa0 *w, s32 a);
BOOL func_ov000_023470ec(UnkStruct_0234bfa0 *w);
void func_ov000_0234ab34(UnkStruct_0234bfa0 *w);
void func_ov000_023457c8(UnkStruct_0234bfa0 *w, s32 a);
void func_ov000_02347190(UnkStruct_0234bfa0 *w);
BOOL func_0206825c(UnkStruct_0234bfa0 *w, void *p);

void func_ov000_0234bfa0(UnkStruct_0234bfa0 *w) {
    u16 f;
    s32 x;

    if (w->unk328 == 4 && (w->unk5C & 8)) {
        Player_SetAnimation(w, 0x3c);
        w->unk5C |= 4;
    }
    if (!(w->unk420 & 0x40) && w->unk328 >= 0x21 && w->unk328 <= 0x22) {
        w->unk5C |= 0x400;
        Player_SetAnimation(w, 0x3c);
        w->unk5C |= 4;
    }
    if (!(w->unk330 & 0x1100) && (w->unk420 & 0x40)) {
        if (w->unk328 < 0x21 || w->unk328 > 0x22) {
            Player_SetAnimation(w, 0x21);
        }
    } else if (w->unk328 >= 0xd && w->unk328 <= 0xe) {
        if (w->unk325 == 0 && !func_ov000_0234be10(w, 0)) {
            w->unk5C |= 0x400;
            Player_SetAnimation(w, 0x3c);
            w->unk5C |= 4;
        }
    } else if (w->unk328 == 0x3c && func_ov000_0234be10(w, 1)) {
        w->unk5C |= 4;
    }
    if (func_ov000_023470ec(w)) {
        w->unk5C &= ~0x400;
        func_ov000_0234ab34(w);
        return;
    }
    if ((w->unk422 & 3) && w->unk340 != NULL) {
        w->unk5C &= ~0x400;
        w->unk340(w);
        return;
    }
    if (!(w->unk330 & 0x1100) && (w->unk420 & 0x80) && w->unk33C != NULL) {
        w->unk5C &= ~0x400;
        w->unk33C(w);
        return;
    }
    if (((w->unk1 + 0x20) & 0xff) >= 0x40) {
        func_ov000_023457c8(w, 0);
    }
    f = w->unk420;
    if (((f & 0x30) || w->unk34 != 0) &&
        (!(w->unk330 & 0x40000) || !(w->unk58 & 4) ||
         ((!(w->unk5C & 1) || !(f & 0x20)) && ((w->unk5C & 1) || !(f & 0x10)))) &&
        (!(w->unk58 & 0xc) ||
         (((x = w->x >> 8) > data_022c4534.unk1C + 0xe || !(f & 0x20)) &&
          (x < data_022c4534.unk24 - 0xe || !(f & 0x10)))) &&
        w->unk338 != NULL) {
        w->unk5C &= ~0x400;
        w->unk338(w);
        return;
    }
    func_ov000_02347190(w);
    if ((w->unk330 & 0x100) && w->unk32 != 0 && w->unk338 != NULL) {
        w->unk5C &= ~0x400;
        w->unk338(w);
        return;
    }
    if (func_0206825c(w, w->unk2A0)) {
        Player_SetAnimation(w, 0x45);
        w->unkC = 0;
        if (w->unk324 != 0) {
            w->unk5C &= ~1;
        }
    }
    if (w->unk324 != 1) {
        if (w->unk328 != 0x45) {
            return;
        }
        if (!(w->unk5C & 8)) {
            return;
        }
        w->unk5C |= 0x400;
        Player_SetAnimation(w, 0x3c);
        w->unk5C |= 4;
        return;
    }
    if (w->unk328 != 0x45) {
        return;
    }
    if (!(w->unk5C & 8)) {
        return;
    }
    Player_SetAnimation(w, 0x47);
    w->unk5C |= 4;
}
