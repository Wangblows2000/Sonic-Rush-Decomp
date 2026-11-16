#include <nitro/types.h>

typedef struct UnkStruct_023466e8 UnkStruct_023466e8;

struct UnkStruct_023466e8 {
    u8 pad0[0x28];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x2E - 0x2C];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    s16 unk62;
    u8 pad64[0x68 - 0x64];
    void (*unk68)(UnkStruct_023466e8 *w);
    u8 pad6C[0x2A0 - 0x6C];
    u8 unk2A0[0x2C4 - 0x2A0];
    u32 unk2C4;
    u8 pad2C8[0x324 - 0x2C8];
    u8 unk324;
    u8 unk325;
    u8 pad326[0x328 - 0x326];
    s16 unk328;
    u8 pad32A[0x32C - 0x32A];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x350 - 0x334];
    s16 unk350;
    u8 pad352[0x368 - 0x352];
    s16 unk368;
    u8 pad36A[0x36C - 0x36A];
    s16 unk36C;
    u8 pad36E[0x384 - 0x36E];
    u8 unk384[0x3BE - 0x384];
    s16 unk3BE;
    u8 pad3C0[0x3CD - 0x3C0];
    u8 unk3CD;
    u8 unk3CE;
    u8 pad3CF[0x3E8 - 0x3CF];
    void *unk3E8;
    u8 pad3EC[0x420 - 0x3EC];
    u16 unk420;
    u16 unk422;
};

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[0x20 - 0x1E];
    u32 unk20;
} UnkStruct_023466e8_4560;

extern UnkStruct_023466e8_4560 gGameState;
extern UnkStruct_023466e8 *gPlayers[];

s32 ObjRect_HitCenterX(void *a, void *b);
s32 ObjRect_HitCenterY(void *a, void *b);
void EffVs_Create(s32 a, s32 b);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void Player_State_InAir(UnkStruct_023466e8 *w);
void Player_StartBoost(UnkStruct_023466e8 *w);
void Player_StopHighSpeed(UnkStruct_023466e8 *w);
void Player_StopBoost(UnkStruct_023466e8 *w);
void func_ov000_02331ae8(UnkStruct_023466e8 *w, s32 a, s32 b);
void EffBlazeBoostFlame_Create(UnkStruct_023466e8 *w);

static inline void Boost_023466e8(UnkStruct_023466e8 *w, s16 *p) {
    s32 a = *p;
    if (a < 0) {
        a = -a;
    }
    if (a < w->unk368) {
        if (w->unk5C & 1) {
            *p = -w->unk368;
        } else {
            *p = w->unk368;
        }
    }
}

void Player_UpdateBoost(UnkStruct_023466e8 *w) {
    s32 b;
    s32 a;
    s32 d;
    s32 c;

    if (w->unk325 != 0) {
        return;
    }
    if ((w->unk32C & 0x10000) && (w->unk422 & 0x100)) {
        EffVs_Create(ObjRect_HitCenterX(gPlayers[0]->unk2A0, gPlayers[1]->unk2A0),
                            ObjRect_HitCenterY(gPlayers[0]->unk2A0, gPlayers[1]->unk2A0));
        Sound_PlaySeqArcEx(w->unk384, 0, 0x40);
        w->unk3BE = 0x10;
        if (w->unk58 & 0x10) {
            Boost_023466e8(w, &w->unk2E);
        } else {
            Boost_023466e8(w, &w->unk34);
        }
        w->unk32C |= 0x20000;
        w->unk62 = 0x44;
    }
    if ((w->unk422 & 0xc00) && (w->unk68 != Player_State_InAir || !(w->unk32C & 2)) && gGameState.unk1C < 9 &&
        gGameState.unk1D < 2 && !(w->unk32C & 0x100000) && !(w->unk58 & 0x10) && w->unk3CE == 0 &&
        !(w->unk32C & 0x400) && w->unk350 >= 0x50 && !(w->unk32C & 0x80)) {
        Player_StartBoost(w);
    }
    if (((w->unk3E8 == NULL || (w->unk330 & 8)) && (w->unk58 & 1) && !(w->unk58 & 0x2000) && !(w->unk58 & 0x8000) &&
         (w->unk328 <= 0x24 || w->unk328 >= 0x30)) ||
        w->unk350 == 0) {
        a = w->unk2A;
        b = w->unk28;
        if (a < 0) {
            a = -a;
        }
        if (b < 0) {
            b = -b;
        }
        if (w->unk36C > b + a) {
            w->unk3CD++;
            if (w->unk3CD > 8 || w->unk350 == 0) {
                Player_StopHighSpeed(w);
            }
        } else {
            w->unk3CD = 0;
        }
    }
    if (!(w->unk420 & 0xc00) || !(w->unk32C & 0x100) || w->unk350 == 0) {
        Player_StopBoost(w);
    }
    if (w->unk32C & 0x80) {
        w->unk2C4 |= 5;
    }
    if (w->unk3E8 == NULL && !(w->unk330 & 8)) {
        if (!(gGameState.unk20 & 7) && (gGameState.unk18 & 1)) {
            c = w->unk2A;
            d = w->unk28;
            if (c < 0) {
                c = -c;
            }
            if (d < 0) {
                d = -d;
            }
            if (d + c > 0x200 && !(w->unk32C & 0x80) && (w->unk32C & 0x100) && (w->unk58 & 1)) {
                if (!(w->unk330 & 0x1000)) {
                    func_ov000_02331ae8(w, -0xd, 0xd);
                } else {
                    func_ov000_02331ae8(w, -0xd, 0);
                }
            }
        }
        if (!(gGameState.unk20 & 3) && (gGameState.unk18 & 1) && w->unk324 == 1 && (w->unk32C & 0x80)) {
            EffBlazeBoostFlame_Create(w);
        }
    }
    if (w->unk3CE != 0) {
        w->unk3CE--;
    }
}
