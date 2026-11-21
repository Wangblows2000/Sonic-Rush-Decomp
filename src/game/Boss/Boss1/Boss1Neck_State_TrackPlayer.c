#include <nitro/types.h>

typedef struct UnkStruct_022c7818 UnkStruct_022c7818;
struct UnkStruct_022c7818 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c7818 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad2[2];
    u16 unk210;
};

typedef struct UnkStruct_022c7818_2 {
    u8 pad[0x21c];
    s16 unk21C;
    s16 unk21E;
    u8 pad1[0x22a - 0x220];
    u16 unk22A;
} UnkStruct_022c7818_2;

typedef struct UnkStruct_022c7818_3 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
} UnkStruct_022c7818_3;

extern UnkStruct_022c7818_2 *gBoss1;
extern UnkStruct_022c7818_3 *gPlayers;
extern u16 data_ov000_02362b8c[];

s32 Boss1Neck_CalcTurretAngle(s32 x, s32 y, s32 z, s32 a);
void Boss1Neck_State_FireAtPlayer(UnkStruct_022c7818 *s);

void Boss1Neck_State_TrackPlayer(UnkStruct_022c7818 *s) {
    UnkStruct_022c7818_2 *g = gBoss1;
    s16 t;
    u16 lim;
    t = Boss1Neck_CalcTurretAngle(gPlayers->unk10, gPlayers->unk14, gPlayers->unk18, 1);
    if (g->unk21C < t) {
        if (t - g->unk21C > 0x100)
            g->unk21C += 0x100;
        else
            g->unk21C = t;
    } else if (g->unk21C > t) {
        if (g->unk21C - t > 0x100)
            g->unk21C -= 0x100;
        else
            g->unk21C = t;
    }
    t = Boss1Neck_CalcTurretAngle(gPlayers->unk10, gPlayers->unk14, gPlayers->unk18, 0);
    if (g->unk21E < t) {
        if (t - g->unk21E > 0x100)
            g->unk21E += 0x100;
        else
            g->unk21E = t;
    } else if (g->unk21E > t) {
        if (g->unk21E - t > 0x100)
            g->unk21E -= 0x100;
        else
            g->unk21E = t;
    }
    lim = data_ov000_02362b8c[gBoss1->unk22A];
    if (gBoss1->unk22A == 3 && (s->unk20C & 7) == 0)
        lim += 0x32;
    if (lim < s->unk210++)
        s->func = Boss1Neck_State_FireAtPlayer;
}
