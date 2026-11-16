#include <nitro/types.h>

typedef struct UnkStruct_022e3954_pl UnkStruct_022e3954_pl;
struct UnkStruct_022e3954_pl {
    u8 pad[0x10];
    s32 unk10;
    u8 pad1[0x30 - 0x14];
    s16 unk30;
    u8 pad2[0x34 - 0x32];
    s16 unk34;
    u8 pad3[0x328 - 0x36];
    s16 unk328;
    u8 pad4[0x32c - 0x32a];
    u32 unk32C;
    u8 pad5[0x338 - 0x330];
    void (*unk338)(UnkStruct_022e3954_pl *p);
};

typedef struct {
    u8 pad[0x20c];
    u32 unk20C;
    u8 pad1[0xd5e - 0x210];
    u16 unkD5E;
    u8 pad2[0xf68 - 0xd60];
    s16 unkF68;
    u8 pad3[0x111c - 0xf6a];
    s32 unk111C;
    u32 unk1120;
    u8 pad4[0x112c - 0x1124];
    s32 unk112C;
} UnkStruct_022e3954;

typedef struct {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022e3954_pad;

typedef struct {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022e3954_4;

extern UnkStruct_022e3954_pl *gPlayers;
extern UnkStruct_022e3954_pad gPad;
extern u16 data_ov000_02363710[];
extern UnkStruct_022e3954_4 gGameState;

s32 func_0200225c(s32 a, s32 b);
void Player_Action_KnockbackIntangible(UnkStruct_022e3954_pl *p, s32 a, s32 b);

void func_ov000_022e3954(UnkStruct_022e3954 *s) {
    s32 i;
    BOOL found;
    BOOL b;
    s32 idx;
    s32 x;
    s32 d;
    s32 v;

    if (gPlayers->unk32C & 0x400)
        return;
    i = 0;
    found = FALSE;
    for (; i < 2; i++) {
        if (gPad.unk4 & data_ov000_02363710[i]) {
            s->unk111C += 0x200;
            found = TRUE;
        }
    }
    if (s->unk112C > 0 && found) {
        s->unk112C--;
        s->unk111C += 0x200;
    }
    b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        idx = 1;
    else
        idx = gGameState.unk14;
    if (idx == 1)
        s->unk111C -= 0x20;
    else
        s->unk111C -= 0x15;
    if (s->unk111C < 0)
        s->unk111C = 0;
    else if (s->unk111C > 0x2000)
        s->unk111C = 0x2000;
    if (found)
        s->unk1120 = 0;
    else
        s->unk1120++;
    if (s->unk1120 >= 0xf)
        s->unk111C = 0;
    x = gPlayers->unk10;
    if (x > 0x34000)
        return;
    if (x < 0)
        return;
    if ((s->unkD5E == 0 && !(s->unk20C & 0x100)) || (s->unkD5E != 0 && (s->unk20C & 0x100)))
        d = 0x31e00 - x;
    else
        d = x - 0x2200;
    if (d < 0)
        d = 0;
    v = func_0200225c(((-((0x2fc00 - d) << 8)) / 0x2fc00 - 0x400) * s->unkF68, 0x549f) >> 12;
    gPlayers->unk34 = s->unk111C * 0x500 / 0x2000;
    if (s->unk20C & 0x100) {
        v *= -1;
        gPlayers->unk34 *= -1;
    }
    if (s->unkD5E != 0) {
        v *= -1;
        gPlayers->unk34 *= -1;
    }
    gPlayers->unk10 += v;
    if (gPlayers->unk34 != 0) {
        if (gPlayers->unk328 < 0x3d || gPlayers->unk328 > 0x42)
            gPlayers->unk338(gPlayers);
    }
    if (gPlayers->unk328 == 0xf)
        return;
    if (gPlayers->unk10 > 0x33800) {
        Player_Action_KnockbackIntangible(gPlayers, 0x500, -0x300);
        gPlayers->unk30 *= 2;
        return;
    }
    if (gPlayers->unk10 < 0x800) {
        Player_Action_KnockbackIntangible(gPlayers, -0x500, -0x300);
        gPlayers->unk30 *= 2;
    }
}
