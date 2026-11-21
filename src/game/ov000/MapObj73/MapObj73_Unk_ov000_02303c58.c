
#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x3C - 4];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    u8 pad4C[0x64 - 0x4C];
} UnkStruct_Plr;

typedef struct {
    u8 pad0[6];
    s8 unk6;
    s8 unk7;
    u8 unk8;
    u8 unk9;
} UnkStruct_Prm;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x210 - 0x20C];
    s32 unk210;
    s32 unk214;
    u8 pad218[0x21C - 0x218];
    s8 unk21C[2];
} UnkStruct_02303930;

extern UnkStruct_Plr gCamera[];
extern UnkStruct_StageObj *gPlayers[];

void MapObj73_Unk_ov000_02303c58(UnkStruct_02303930 *s) {
    UnkStruct_Plr *P;
    u8 i;
    s32 y;
    s32 x;

    for (i = 0; i < 2; i++) {
        P = &gCamera[i];
        if (gPlayers[s->unk21C[i]]->unk10 > s->base.unk10 + ((((UnkStruct_Prm *)s->base.unk1E0)->unk6 + ((UnkStruct_Prm *)s->base.unk1E0)->unk8) << 8)) {
            P->unk3C = s->unk210;
            P->unk40 = s->unk214;
            P->unk0 &= ~0x2000;
            P->unk0 |= 0x1000;
        } else if (gPlayers[s->unk21C[i]]->unk10 < s->base.unk10 + (s16)(((UnkStruct_Prm *)s->base.unk1E0)->unk6 << 8)) {
            P->unk0 &= ~0x2000;
            P->unk0 &= ~0x1000;
        } else {
            P->unk0 &= ~0x1000;
            if (gPlayers[s->unk21C[i]]->unk14 >= s->base.unk14 + (s16)(((UnkStruct_Prm *)s->base.unk1E0)->unk7 << 8) &&
                gPlayers[s->unk21C[i]]->unk14 <= s->base.unk14 + ((((UnkStruct_Prm *)s->base.unk1E0)->unk7 + ((UnkStruct_Prm *)s->base.unk1E0)->unk9) << 8)) {
                P->unk0 |= 0x2000;
                y = P->unk48 + (s->unk214 - P->unk48) * ((gPlayers[s->unk21C[i]]->unk10 >> 8) - ((s->base.unk10 >> 8) + ((UnkStruct_Prm *)s->base.unk1E0)->unk6)) / ((UnkStruct_Prm *)s->base.unk1E0)->unk8;
                x = P->unk44 + (s->unk210 - P->unk44) * ((gPlayers[s->unk21C[i]]->unk10 >> 8) - ((s->base.unk10 >> 8) + ((UnkStruct_Prm *)s->base.unk1E0)->unk6)) / ((UnkStruct_Prm *)s->base.unk1E0)->unk8;
                P->unk3C = x;
                P->unk40 = y;
            }
        }
    }
}
